#include "CameraManager.h"
#include <mfcaptureengine.h>
#include <mfobjects.h>
#include <atlbase.h>
#include <vector>
#include <winerror.h>

CameraManager::CameraManager()
    : mediaSource(nullptr), sourceReader(nullptr), devices(nullptr),
      deviceCount(0), currentWidth(0), currentHeight(0), currentFramerate(0) {
    InitializeMediaFoundation();
}

CameraManager::~CameraManager() {
    Stop();
    
    if (devices) {
        for (UINT32 i = 0; i < deviceCount; ++i) {
            devices[i]->Release();
        }
        CoTaskMemFree(devices);
    }
    
    MFShutdown();
}

bool CameraManager::InitializeMediaFoundation() {
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) return false;
    
    hr = MFStartup(MF_VERSION);
    return SUCCEEDED(hr);
}

std::vector<CameraInfo> CameraManager::EnumerateCameras() {
    std::vector<CameraInfo> cameras;
    
    IMFAttributes* attributes = nullptr;
    HRESULT hr = MFCreateAttributes(&attributes, 1);
    if (FAILED(hr)) return cameras;
    
    hr = attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                             MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    if (FAILED(hr)) {
        attributes->Release();
        return cameras;
    }
    
    IMFActivate** ppDevices = nullptr;
    UINT32 count = 0;
    hr = MFEnumDeviceSources(attributes, &ppDevices, &count);
    attributes->Release();
    
    if (SUCCEEDED(hr)) {
        for (UINT32 i = 0; i < count; ++i) {
            cameras.push_back(GetCameraInfo(ppDevices[i]));
            ppDevices[i]->Release();
        }
        CoTaskMemFree(ppDevices);
    }
    
    return cameras;
}

CameraInfo CameraManager::GetCameraInfo(IMFActivate* device) {
    CameraInfo info;
    
    // Get device name
    wchar_t name[256] = {};
    UINT32 nameLen = 0;
    if (SUCCEEDED(device->GetString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, name, 256, &nameLen))) {
        info.name = std::wstring(name);
    }
    
    // Get device ID
    wchar_t deviceId[256] = {};
    UINT32 idLen = 0;
    if (SUCCEEDED(device->GetString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, deviceId, 256, &idLen))) {
        info.deviceId = std::wstring(deviceId);
    }
    
    // For now, add some common resolutions
    // In a full implementation, we'd query these from the device
    info.resolutions = {
        {640, 480},
        {800, 600},
        {1024, 768},
        {1280, 720},
        {1920, 1080}
    };
    
    info.framerates = {15, 24, 30, 60};
    
    return info;
}

bool CameraManager::InitializeFirstCamera() {
    auto cameras = EnumerateCameras();
    if (cameras.empty()) return false;
    
    // Use first camera with 1280x720 at 30fps as default
    return InitializeCamera(cameras[0].deviceId, 1280, 720, 30);
}

bool CameraManager::InitializeCamera(const std::wstring& deviceId, int width, int height, int framerate) {
    Stop();
    
    IMFAttributes* attributes = nullptr;
    HRESULT hr = MFCreateAttributes(&attributes, 2);
    if (FAILED(hr)) return false;
    
    attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    attributes->SetString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, deviceId.c_str());
    
    hr = MFCreateDeviceSource(attributes, &mediaSource);
    attributes->Release();
    
    if (FAILED(hr)) return false;
    
    // Create source reader with format conversion enabled
    attributes = nullptr;
    hr = MFCreateAttributes(&attributes, 1);
    if (SUCCEEDED(hr)) {
        attributes->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, FALSE);
        
        hr = MFCreateSourceReaderFromMediaSource(mediaSource, attributes, &sourceReader);
        attributes->Release();
    }
    
    if (FAILED(hr)) return false;
    
    // Find and set the first available video stream
    // Try multiple media types to find one that works
    DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM;
    
    // Try to enumerate and use the first available format
    for (DWORD typeIndex = 0; typeIndex < 10; typeIndex++) {
        IMFMediaType* mediaType = nullptr;
        hr = sourceReader->GetNativeMediaType(streamIndex, typeIndex, &mediaType);
        
        if (SUCCEEDED(hr) && mediaType) {
            // Try to set this as the current type
            sourceReader->SetCurrentMediaType(streamIndex, nullptr, mediaType);
            
            // Query the actual dimensions
            UINT32 w = 0, h = 0;
            if (SUCCEEDED(MFGetAttributeSize(mediaType, MF_MT_FRAME_SIZE, &w, &h))) {
                currentWidth = (w > 0) ? w : width;
                currentHeight = (h > 0) ? h : height;
            } else {
                currentWidth = width;
                currentHeight = height;
            }
            
            mediaType->Release();
            currentFramerate = framerate;
            return true;
        }
        
        if (mediaType) mediaType->Release();
    }
    
    // If we couldn't find a specific type, try without setting one
    // The reader might use the default
    currentWidth = width;
    currentHeight = height;
    currentFramerate = framerate;
    return true;
}

void CameraManager::Stop() {
    if (sourceReader) {
        sourceReader->Release();
        sourceReader = nullptr;
    }
    
    if (mediaSource) {
        mediaSource->Shutdown();
        mediaSource->Release();
        mediaSource = nullptr;
    }
}

CameraInfo CameraManager::GetCurrentCameraInfo() const {
    CameraInfo info;
    return info;
}

bool CameraManager::SetResolution(int width, int height) {
    currentWidth = width;
    currentHeight = height;
    return true;
}

bool CameraManager::SetFramerate(int fps) {
    currentFramerate = fps;
    return true;
}

std::vector<std::pair<int, int>> CameraManager::GetSupportedResolutions() {
    return {{640, 480}, {1280, 720}, {1920, 1080}};
}

std::vector<int> CameraManager::GetSupportedFramerates() {
    return {15, 24, 30, 60};
}

wxBitmap CameraManager::GetCurrentFrame() {
    if (!sourceReader) {
        return wxBitmap();
    }
    
    // Try to capture a frame
    CaptureFrame();
    
    return lastFrame;
}

bool CameraManager::CaptureFrame() {
    if (!sourceReader) return false;
    
    DWORD streamIndex = 0;
    DWORD flags = 0;
    LONGLONG timestamp = 0;
    IMFSample* sample = nullptr;
    
    // ReadSample should block until a sample is available or error occurs
    // The timeout is indefinite by default
    HRESULT hr = sourceReader->ReadSample(
        (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
        0,  // No flags - this means blocking read
        &streamIndex,
        &flags,
        &timestamp,
        &sample
    );
    
    // Check error
    if (FAILED(hr)) {
        return false;
    }
    
    // Check if stream ended or needs format change
    if (flags & MF_SOURCE_READERF_STREAMTICK) {
        // Just a stream tick, no actual sample
        if (sample) sample->Release();
        return false;
    }
    
    if (flags & MF_SOURCE_READERF_ENDOFSTREAM) {
        // End of stream
        if (sample) sample->Release();
        return false;
    }
    
    if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED) {
        // Media type changed - re-query it
        IMFMediaType* currentType = nullptr;
        if (SUCCEEDED(sourceReader->GetCurrentMediaType(
            (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            &currentType))) {
            UINT32 w = 0, h = 0;
            MFGetAttributeSize(currentType, MF_MT_FRAME_SIZE, &w, &h);
            if (w > 0 && h > 0) {
                currentWidth = w;
                currentHeight = h;
            }
            currentType->Release();
        }
        if (sample) sample->Release();
        return false;
    }
    
    // No sample available yet
    if (!sample) {
        return false;
    }
    
    // Convert sample to wxBitmap
    lastFrame = ConvertMediaFoundationToWxBitmap(sample);
    sample->Release();
    
    return lastFrame.IsOk();
}

wxBitmap CameraManager::ConvertMediaFoundationToWxBitmap(IMFSample* sample) {
    if (!sample) return wxBitmap();
    
    IMFMediaBuffer* buffer = nullptr;
    HRESULT hr = sample->GetBufferByIndex(0, &buffer);
    if (FAILED(hr) || !buffer) return wxBitmap();
    
    BYTE* data = nullptr;
    DWORD maxLen = 0, currentLen = 0;
    
    hr = buffer->Lock(&data, &maxLen, &currentLen);
    if (FAILED(hr)) {
        buffer->Release();
        return wxBitmap();
    }
    
    int width = currentWidth > 0 ? currentWidth : 640;
    int height = currentHeight > 0 ? currentHeight : 480;
    
    // Validate we have data
    if (!data || currentLen == 0) {
        buffer->Unlock();
        buffer->Release();
        return wxBitmap();
    }
    
    wxBitmap result;
    
    // Calculate expected size for different formats
    // RGB32: 4 bytes per pixel
    // NV12: 1.5 bytes per pixel (Y plane + interleaved UV)
    // YUY2: 2 bytes per pixel
    
    int expectedRGB32Size = width * height * 4;
    int expectedNV12Size = width * height * 3 / 2;
    int expectedYUY2Size = width * height * 2;
    
    // Try to detect format and convert
    if (currentLen >= (DWORD)expectedRGB32Size) {
        // Try RGB32 format first (most likely if we requested it)
        result = ConvertRGB32ToWxBitmap(data, width, height);
        if (result.IsOk()) {
            buffer->Unlock();
            buffer->Release();
            return result;
        }
    }
    
    // Try NV12 format (common camera format)
    if (currentLen >= (DWORD)expectedNV12Size) {
        result = ConvertNV12ToWxBitmap(data, width, height);
        if (result.IsOk()) {
            buffer->Unlock();
            buffer->Release();
            return result;
        }
    }
    
    // Try YUY2 format (another common camera format)
    if (currentLen >= (DWORD)expectedYUY2Size && currentLen < (DWORD)expectedRGB32Size) {
        result = ConvertYUY2ToWxBitmap(data, width, height);
        if (result.IsOk()) {
            buffer->Unlock();
            buffer->Release();
            return result;
        }
    }
    
    buffer->Unlock();
    buffer->Release();
    
    return wxBitmap();
}

wxBitmap CameraManager::ConvertRGB32ToWxBitmap(BYTE* data, int width, int height) {
    if (!data || width <= 0 || height <= 0) {
        return wxBitmap();
    }
    
    try {
        wxImage image(width, height);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int idx = (y * width + x) * 4;
                // BGRA format in memory
                BYTE b = data[idx];
                BYTE g = data[idx + 1];
                BYTE r = data[idx + 2];
                // A = data[idx + 3];  // Alpha channel, not used
                
                image.SetRGB(x, y, r, g, b);
            }
        }
        
        return wxBitmap(image);
    } catch (...) {
        return wxBitmap();
    }
}

wxBitmap CameraManager::ConvertNV12ToWxBitmap(BYTE* data, int width, int height) {
    if (!data || width <= 0 || height <= 0) {
        return wxBitmap();
    }
    
    try {
        wxImage image(width, height);
        
        // NV12 format: Y plane followed by interleaved UV plane
        // Y plane: width * height bytes
        // UV plane: (width/2) * (height/2) * 2 bytes (interleaved U and V)
        
        int ySize = width * height;
        BYTE* yPlane = data;
        BYTE* uvPlane = data + ySize;
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int yIdx = y * width + x;
                int Y = yPlane[yIdx];
                
                // UV coordinates (each UV value covers 2x2 Y pixels)
                int uvX = x / 2;
                int uvY = y / 2;
                int uvIdx = (uvY * (width / 2) + uvX) * 2;
                
                int U = uvPlane[uvIdx];
                int V = uvPlane[uvIdx + 1];
                
                // YUV to RGB conversion
                int C = Y - 16;
                int D = U - 128;
                int E = V - 128;
                
                int R = (298 * C + 409 * E + 128) >> 8;
                int G = (298 * C - 100 * D - 208 * E + 128) >> 8;
                int B = (298 * C + 516 * D + 128) >> 8;
                
                // Clamp to valid range
                R = (R < 0) ? 0 : (R > 255) ? 255 : R;
                G = (G < 0) ? 0 : (G > 255) ? 255 : G;
                B = (B < 0) ? 0 : (B > 255) ? 255 : B;
                
                image.SetRGB(x, y, R, G, B);
            }
        }
        
        return wxBitmap(image);
    } catch (...) {
        return wxBitmap();
    }
}

wxBitmap CameraManager::ConvertYUY2ToWxBitmap(BYTE* data, int width, int height) {
    if (!data || width <= 0 || height <= 0) {
        return wxBitmap();
    }
    
    try {
        wxImage image(width, height);
        
        // YUY2 format: 4 bytes per 2 pixels
        // Layout: Y0 U Y1 V Y2 U Y3 V ...
        // So each 4-byte group covers 2 horizontal pixels
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Find the byte position
                int bytePos = (y * width + x) * 2;
                
                int Y, U, V;
                if (x % 2 == 0) {
                    // Even pixel: Y0 at bytePos, U at bytePos+1
                    Y = data[bytePos];
                    U = data[bytePos + 1];
                    V = data[bytePos + 3];
                } else {
                    // Odd pixel: Y1 at bytePos, V at bytePos+1
                    Y = data[bytePos];
                    U = data[bytePos - 1];
                    V = data[bytePos + 1];
                }
                
                // YUV to RGB conversion (ITU-R BT.601)
                int C = Y - 16;
                int D = U - 128;
                int E = V - 128;
                
                int R = (298 * C + 409 * E + 128) >> 8;
                int G = (298 * C - 100 * D - 208 * E + 128) >> 8;
                int B = (298 * C + 516 * D + 128) >> 8;
                
                // Clamp to valid range
                R = (R < 0) ? 0 : (R > 255) ? 255 : R;
                G = (G < 0) ? 0 : (G > 255) ? 255 : G;
                B = (B < 0) ? 0 : (B > 255) ? 255 : B;
                
                image.SetRGB(x, y, R, G, B);
            }
        }
        
        return wxBitmap(image);
    } catch (...) {
        return wxBitmap();
    }
}
