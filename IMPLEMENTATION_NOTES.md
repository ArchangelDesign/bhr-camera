# BHR Camera - Implementation Notes

## Current State

This is the **skeleton/framework** of the camera application. The basic structure is in place with all major components defined and stubbed out. The application can be built but won't display video yet.

## What's Working

1. **Window Management**: Creates a borderless wxWidgets window
2. **Event Handling**: Double-click detection works
3. **Settings Dialog**: Opens with camera/resolution/framerate options
4. **Camera Enumeration**: Discovers cameras via Windows Media Foundation

## What's NOT Yet Implemented

### 1. Video Frame Capture & Rendering

The `CameraManager` needs:
- Actual media type negotiation with the camera
- Setting resolution and framerate on the media source
- Reading samples from the source reader
- Converting media samples to displayable format (RGB32 or similar)

The `MainFrame` needs:
- A rendering surface (wxDC or Direct3D)
- Code to display captured frames
- Possibly a background thread for capture

### 2. Format Negotiation

Currently hardcoded resolution/framerate lists. Needs:
- Enumerate media types from each camera
- Query actual supported formats
- Match user selection to media type GUID

### 3. Camera Settings Dialog

The "Camera Settings..." button needs to:
- Get camera property pages from the driver
- Launch them in a native dialog
- Or expose individual properties (IAMCameraControl interface)

## Implementation Path - Quick Start

### Phase 1: Get Video Rendering Working (Priority: HIGH)

```cpp
// In CameraManager.cpp, modify InitializeCamera to:
// 1. Create a suitable media type (video format)
// 2. Set it on the source reader
// 3. Start reading frames

// In MainFrame.cpp, add:
// 1. A wxMemoryDC or Direct3D surface for drawing
// 2. Periodic timer to call CameraManager->GetFrame()
// 3. OnPaint handler to display the frame
```

### Phase 2: Proper Resolution/Framerate Selection

```cpp
// In CameraManager.cpp:
// - Query IMFSourceReader for available media types
// - Parse each media type for resolution/framerate
// - Build real list of supported formats
// - Apply selected format before streaming
```

### Phase 3: Camera Properties Dialog

```cpp
// Option A: Use IAMCameraControl (manual)
// - Enumerate properties available on camera
// - Create UI for each property
// - Update values via SetValue()

// Option B: Use DirectShow graph (harder but complete)
// - Create filter graph
// - Add capture filter
// - Use OleCreatePropertyFrame()
```

## Code Snippets for Implementation

### Reading Video Frames

```cpp
bool CameraManager::ReadFrame(unsigned char*& buffer, int& width, int& height) {
    if (!sourceReader) return false;
    
    IMFSample* sample = nullptr;
    DWORD flags = 0;
    LONGLONG timestamp = 0;
    
    HRESULT hr = sourceReader->ReadSample(
        (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
        0, &flags, &timestamp, &sample);
    
    if (FAILED(hr) || !sample) return false;
    
    // Convert sample to displayable format
    // Extract buffer and copy to output
    // sample->Release();
    return true;
}
```

### Setting Media Type

```cpp
bool CameraManager::SetMediaType(int width, int height, int fps) {
    IMFMediaType* mediaType = nullptr;
    MFCreateMediaType(&mediaType);
    
    mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    mediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
    mediaType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
    MFSetAttributeSize(mediaType, MF_MT_FRAME_SIZE, width, height);
    MFSetAttributeRatio(mediaType, MF_MT_FRAME_RATE, fps, 1);
    
    HRESULT hr = sourceReader->SetCurrentMediaType(
        (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
        nullptr, mediaType);
    
    mediaType->Release();
    return SUCCEEDED(hr);
}
```

## Dependencies Summary

- **wxWidgets**: UI framework (must be installed separately)
- **Windows Media Foundation**: Built into Windows 7+ (mfplat.lib, mfreadwrite.lib)
- **strmiids.lib**: For COM interfaces

## Binary Size Optimization

Current optimizations:
- Link-time code generation (/LTCG)
- Whole program optimization (/GL)
- Release build (/O2)

Additional options if needed:
- Use UPX to compress executable
- Link wxWidgets statically but use minimal build
- Strip all debug symbols
- Consider using MinGW instead of MSVC

## Testing Recommendations

1. **Build first** without video rendering to verify no compile errors
2. **Test camera enumeration** - add debug output to show found cameras
3. **Gradually add rendering** - start with simple color-fill rectangle
4. **Add timer** for frame updates (~30ms for 30fps)
5. **Test actual video** with real camera hardware

## Common Issues & Solutions

| Issue | Solution |
|-------|----------|
| Camera not found | Check Device Manager, ensure camera is USB or PCI device |
| "No media types" | Media type negotiation failed; check format compatibility |
| Frames not rendering | Background thread communication issue; use wxThreadSafeQueue |
| App crashes on close | Ensure all COM objects are released in destructor |
