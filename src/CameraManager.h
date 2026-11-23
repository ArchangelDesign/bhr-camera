#pragma once

#include <vector>
#include <string>
#include <memory>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <wx/wx.h>

struct CameraInfo {
    std::wstring name;
    std::wstring deviceId;
    std::vector<std::pair<int, int>> resolutions;  // width, height pairs
    std::vector<int> framerates;
};

class CameraManager {
public:
    CameraManager();
    ~CameraManager();
    
    // List all available cameras
    std::vector<CameraInfo> EnumerateCameras();
    
    // Initialize and start streaming from first camera
    bool InitializeFirstCamera();
    
    // Initialize and start streaming from specific camera
    bool InitializeCamera(const std::wstring& deviceId, int width, int height, int framerate);
    
    // Stop streaming
    void Stop();
    
    // Get current camera info
    CameraInfo GetCurrentCameraInfo() const;
    
    // Update camera settings
    bool SetResolution(int width, int height);
    bool SetFramerate(int fps);
    
    // Get list of supported resolutions for current camera
    std::vector<std::pair<int, int>> GetSupportedResolutions();
    
    // Get list of supported framerates for current camera
    std::vector<int> GetSupportedFramerates();
    
    // Get current frame as wxBitmap for display
    wxBitmap GetCurrentFrame();
    
private:
    IMFMediaSource* mediaSource;
    IMFSourceReader* sourceReader;
    IMFActivate** devices;
    UINT32 deviceCount;
    int currentWidth;
    int currentHeight;
    int currentFramerate;
    wxBitmap lastFrame;
    
    bool InitializeMediaFoundation();
    CameraInfo GetCameraInfo(IMFActivate* device);
    bool CaptureFrame();
    wxBitmap ConvertMediaFoundationToWxBitmap(IMFSample* sample);
};
