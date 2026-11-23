# BHR Camera - Quick Reference

## Project at a Glance

| Aspect | Details |
|--------|---------|
| **Language** | C++ (C++17) |
| **Framework** | wxWidgets 3.1+ |
| **Camera API** | Windows Media Foundation |
| **Platform** | Windows (Linux/macOS ready) |
| **Binary Size** | 8-15 MB (optimized) |
| **Build System** | CMake + Visual Studio |
| **Dependencies** | wxWidgets (external), WMF (built-in) |

## File Organization

```
Core Components:
  - MainFrame: UI window (borderless, resizable)
  - CameraManager: Camera hardware control
  - SettingsDialog: Configuration interface
  - main.cpp: Application startup

Build Files:
  - CMakeLists.txt: CMake configuration
  - build.bat: Windows build script

Documentation:
  - README.md: Setup & usage
  - ARCHITECTURE.md: Technical design
  - IMPLEMENTATION_NOTES.md: Code snippets & guidance
  - CHECKLIST.md: Status & TODOs
  - PROJECT_SUMMARY.md: Overview
```

## Keyboard Shortcuts (Future)

- **Double-click**: Open settings
- **ESC**: (future) Close app/exit fullscreen
- **Right-click**: (future) Context menu

## Build Steps

1. Install wxWidgets (see README.md)
2. Run `build.bat`
3. Find executable in `build\Release\`

## Key Classes & Methods

### MainFrame
```cpp
MainFrame(wxWindow* parent);
OnDoubleClick(wxMouseEvent&);     // Opens settings
OnFrameSizeChanged(wxSizeEvent&); // Resizes panel
OnPaint(wxPaintEvent&);           // Renders video
```

### CameraManager
```cpp
EnumerateCameras();               // List all cameras
InitializeFirstCamera();          // Start default
InitializeCamera(...);            // Start specific camera
GetSupportedResolutions();        // Query formats
GetSupportedFramerates();         // Query fps options
SetResolution(w, h);              // Change resolution
SetFramerate(fps);                // Change framerate
```

### SettingsDialog
```cpp
SettingsDialog(parent, cameraManager);
// Auto-populated dropdowns for camera/resolution/fps
// OK button applies changes, Cancel discards
```

## Windows Media Foundation Quick Facts

- **Main interfaces used**:
  - `IMFActivate` - Device enumeration
  - `IMFMediaSource` - Hardware interface
  - `IMFSourceReader` - Frame reading
  - `IMFMediaType` - Format negotiation

- **To enumerate cameras**:
  - Create MF attributes with `MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID`
  - Call `MFEnumDeviceSources()`
  - Iterate results for device properties

- **To read frames**:
  - Create `IMFSourceReader`
  - Set media type (resolution, format, fps)
  - Call `ReadSample()` in loop
  - Convert sample to displayable format

## Common Error Codes

| Code | Meaning | Solution |
|------|---------|----------|
| `MF_E_NOT_INITIALIZED` | WMF not ready | Call `MFStartup()` |
| `E_FAIL` | Camera not found | Check Device Manager |
| `MF_E_INVALIDREQUEST` | Bad media type | Check resolution/fps support |
| `S_OK` | Success | Continue normally |

## Configuration Files (Future)

Will likely be stored in:
- Windows: `%APPDATA%\BHRCamera\settings.ini`
- Linux: `~/.config/bhrcamera/settings.ini`
- macOS: `~/Library/Preferences/BHRCamera/settings.ini`

## Performance Targets

- **Startup time**: < 1 second
- **Camera init**: < 2 seconds
- **Frame rate**: 30 FPS native (configurable)
- **Memory usage**: < 100 MB
- **CPU usage**: < 15% at 30 FPS
- **Binary size**: < 20 MB

## Testing Checklist

- [ ] Builds without errors
- [ ] Launches without crashing
- [ ] Detects at least one camera
- [ ] Settings dialog opens on double-click
- [ ] Camera/resolution/fps dropdowns work
- [ ] Video displays in window
- [ ] Window resizing works
- [ ] Can switch cameras
- [ ] Can change resolution
- [ ] Can change framerate
- [ ] Clean shutdown (no memory leaks)

## Debug Tips

**Show all cameras found**:
```cpp
// In MainFrame::MainFrame, after creating CameraManager:
auto cameras = cameraManager->EnumerateCameras();
for (auto& cam : cameras) {
    wxLogMessage("Camera: %s", cam.name);
}
```

**Show Windows device info**:
```powershell
Get-PnpDevice -Class Camera
```

**Monitor CPU/Memory**:
- Task Manager → Performance tab
- Run with `-debug` flag (future)

## Resources

- **wxWidgets**: https://wxwidgets.org/
- **WMF Docs**: https://docs.microsoft.com/windows/win32/medfound/
- **Build**: See README.md for detailed setup
- **Code**: See IMPLEMENTATION_NOTES.md for snippets

## Support & Troubleshooting

If build fails:
1. Check wxWidgets installation
2. Verify Visual Studio version
3. Run CMake manually with verbose output
4. Check IMPLEMENTATION_NOTES.md

If camera not found:
1. Check Device Manager
2. Test with Windows Camera app
3. Verify camera drivers installed
4. Try different USB port

If video not rendering:
1. This is NOT yet implemented (Phase 1)
2. See CHECKLIST.md for status
3. See IMPLEMENTATION_NOTES.md for how to add it

---

**Last Updated**: Initial project creation
**Status**: Framework complete, ready for development
