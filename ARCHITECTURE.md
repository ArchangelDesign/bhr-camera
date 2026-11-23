# BHR Camera - Project Architecture

## Overview

A lightweight camera streaming application built with C++ and wxWidgets for Windows (with cross-platform design).

## Component Structure

### 1. MainFrame (src/MainFrame.h/cpp)
**Purpose**: Main application window that displays the camera stream
- Borderless window with no decorations (wxFRAME_NO_BORDER)
- Full-screen capable and resizable
- Double-click event handler opens settings dialog
- Event table for handling mouse and paint events
- Communicates with CameraManager for camera control

### 2. CameraManager (src/CameraManager.h/cpp)
**Purpose**: Manages camera enumeration and control using Windows Media Foundation
- Initializes COM and Media Foundation
- Enumerates available cameras with friendly names and device IDs
- Collects supported resolutions and framerates from devices
- Manages media source and source reader instances
- Methods for switching cameras, changing resolution/framerate
- Handles graceful shutdown and cleanup

### 3. SettingsDialog (src/SettingsDialog.h/cpp)
**Purpose**: Settings panel accessed via double-click
- Three combo boxes: Camera, Resolution, Framerate
- Dynamically populates options based on camera capabilities
- "Camera Settings..." button (placeholder for driver settings dialog)
- OK/Cancel buttons to apply or discard changes
- Communicates changes back to CameraManager

### 4. BHRCameraApp (src/main.cpp)
**Purpose**: Application entry point
- Simple wxApp subclass that creates and shows MainFrame

## Key Design Decisions

### Why wxWidgets?
- Minimal dependencies compared to Qt
- Small binary footprint (~8-15 MB optimized)
- Native look and feel on each platform
- Cross-platform support (Windows, Linux, macOS)
- Mature, stable API

### Why Windows Media Foundation?
- Modern replacement for deprecated DirectShow
- Direct access to camera hardware
- Support for format negotiation and framerate control
- Built into Windows (no additional dependencies)

### Borderless Window
- wxFRAME_NO_BORDER flag removes all window chrome
- Allows 100% of window space for video content
- Double-click to access settings ensures minimal UI intrusion

## Build System

- CMake for cross-platform compatibility
- Visual Studio project generation on Windows
- Optimization flags for minimal binary size:
  - /O2 (optimization for speed)
  - /GL (whole program optimization)
  - /LTCG (link-time code generation)

## Threading Considerations

Currently single-threaded UI. For production, consider:
- Moving camera streaming to background thread
- Using wxThread for safe cross-thread communication
- Event queuing for frame updates

## Future Enhancements

1. **Video Rendering**: Implement actual video frame display (currently stub)
2. **Camera Driver Settings**: Launch native camera control panels
3. **Recording**: Add video recording capability
4. **Multi-camera**: Support displaying multiple camera streams
5. **Linux Support**: Add V4L2 camera backend
6. **macOS Support**: Add AVFoundation camera backend
