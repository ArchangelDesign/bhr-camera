# BHR Camera - Implementation Checklist

## ✅ Completed

### Project Structure
- [x] Created src/ directory structure
- [x] CMakeLists.txt with CMake configuration
- [x] .gitignore with appropriate exclusions
- [x] README.md with build and usage instructions
- [x] ARCHITECTURE.md with technical overview

### Core Components
- [x] MainFrame - Borderless window with double-click settings access
- [x] CameraManager - Camera enumeration and Media Foundation integration
- [x] SettingsDialog - Camera, resolution, and framerate selection
- [x] Application entry point (main.cpp)

### Features Implemented
- [x] Window resizing with video panel
- [x] Borderless window (no decorations)
- [x] Double-click detection for settings dialog
- [x] Camera enumeration from Windows Media Foundation
- [x] Settings dialog with combo boxes for:
  - [x] Camera selection
  - [x] Resolution selection
  - [x] Framerate selection
  - [x] Camera settings button (placeholder)

## ⏳ To Do - Core Functionality

### Video Rendering
- [ ] Implement actual video frame capture in CameraManager
- [ ] Set up media format negotiation (resolution/framerate)
- [ ] Create video rendering pipeline (D3D or GDI+)
- [ ] Display frames in MainFrame video panel

### Camera Driver Settings
- [ ] Implement "Camera Settings..." button functionality
- [ ] Launch native Windows camera control dialog

### Configuration & State
- [ ] Save/restore last selected camera and settings
- [ ] Configuration file (INI or JSON)

## ⏳ To Do - Polish & Testing

### Testing
- [ ] Test with actual camera hardware
- [ ] Test resolution/framerate switching
- [ ] Test camera disconnection/reconnection
- [ ] Error handling for missing cameras

### Performance
- [ ] Profile binary size
- [ ] Optimize for minimal disk footprint
- [ ] Test framerate performance

### UI/UX
- [ ] Add proper error messages
- [ ] Implement actual camera driver settings dialog
- [ ] Add status display (current camera, resolution, FPS)

## ⏳ To Do - Future Platforms

### Linux Support
- [ ] Switch from WMF to V4L2 or libcamera
- [ ] Adapt UI code for GTK backend

### macOS Support
- [ ] Switch from WMF to AVFoundation
- [ ] Adapt UI code for Cocoa backend

## Build Status

**Current State**: Project structure complete, components stubbed out

**Next Step**: Implement actual video frame capture and rendering in CameraManager
