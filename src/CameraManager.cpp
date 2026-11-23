#include "CameraManager.h"
#include <mfcaptureengine.h>
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
    
    // Create source reader
    attributes = nullptr;
    hr = MFCreateAttributes(&attributes, 1);
    if (SUCCEEDED(hr)) {
        attributes->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, FALSE);
        
        hr = MFCreateSourceReaderFromMediaSource(mediaSource, attributes, &sourceReader);
        attributes->Release();
    }
    
    if (SUCCEEDED(hr)) {
        currentWidth = width;
        currentHeight = height;
        currentFramerate = framerate;
        return true;
    }
    
    return false;
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
    
    HRESULT hr = sourceReader->ReadSample(
        (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
        0,
        &streamIndex,
        &flags,
        &timestamp,
        &sample
    );
    
    if (FAILED(hr)) return false;
    if (!sample) return false;
    
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
    
    // Create a simple test bitmap (placeholder)
    // In a real implementation, we'd decode the actual camera data format
    wxBitmap bitmap(currentWidth > 0 ? currentWidth : 640, 
                   currentHeight > 0 ? currentHeight : 480);
    
    // Create a memory device context and draw placeholder
    wxMemoryDC dc(bitmap);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawRectangle(0, 0, bitmap.GetWidth(), bitmap.GetHeight());
    
    // Draw a simple test pattern (you'll replace this with actual video decoding)
    dc.SetTextForeground(*wxWHITE);
    dc.DrawText("Camera Stream", 10, 10);
    
    buffer->Unlock();
    buffer->Release();
    
    return bitmap;
}
