# BHR Camera

A lightweight, minimal-dependency camera streaming application built with C++ and wxWidgets.

## Features

- **Minimal Binary Size**: Optimized for small executable size (~8-15 MB)
- **Borderless Window**: Full-screen camera view with no UI chrome
- **Camera Selection**: Choose from multiple cameras
- **Resolution Control**: Select from camera-supported resolutions
- **Framerate Control**: Adjust streaming framerate
- **Camera Settings**: Access camera driver settings (auto-focus, exposure, etc.)
- **Cross-Platform Design**: Built with wxWidgets for future Linux/macOS support

## Requirements

- **Windows 10 or later**
- **Visual Studio 2017 or later** (for MSVC compiler)
- **CMake 3.10 or later**
- **wxWidgets 3.1 or later** (must be built from source or pre-built binaries)

## Setup

### 1. Install wxWidgets

#### Option A: Pre-built binaries (Easiest)
Download from https://www.wxwidgets.org/downloads/

#### Option B: Build from source
```bash
git clone https://github.com/wxWidgets/wxWidgets.git
cd wxWidgets
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cmake --install . --prefix C:\wxWidgets-install
```

### 2. Configure CMake

Create a build directory and configure:

```bash
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A x64 ^
  -DwxWidgets_ROOT_DIR="C:\path\to\wxWidgets" ^
  -DwxWidgets_LIB_DIR="C:\path\to\wxWidgets\lib\vc_x64_lib" ^
  ..
```

Replace paths with your wxWidgets installation.

### 3. Build

```bash
cmake --build . --config Release
```

The executable will be in `Release/` directory.

## Usage

### Run the Application

```bash
bhr-camera.exe
```

### Controls

- **Double-click** the window to open the Settings dialog
- Select camera, resolution, and framerate
- Click "Camera Settings..." to access driver-specific controls
- Click "OK" to apply changes

## Project Structure

```
src/
  main.cpp                 # Application entry point
  MainFrame.h/cpp          # Main window implementation
  SettingsDialog.h/cpp     # Settings dialog implementation
  CameraManager.h/cpp      # Camera enumeration and control
```

## Building for Minimal Size

The CMakeLists.txt includes optimization flags:
- `/O2` (optimized for speed)
- `/GL` (whole program optimization)
- `/LTCG` (link-time code generation)

To further reduce binary size, you can:
1. Use UPX (executable packer)
2. Strip debugging symbols
3. Consider static linking wxWidgets

## Technical Details

### Camera API
- Uses **Windows Media Foundation** (MF) for camera access
- Avoids deprecated DirectShow API
- Modern, low-level camera control

### UI Framework
- **wxWidgets**: Lightweight, native controls, minimal dependencies
- Borderless window mode for full-screen video display

## Future Enhancements

- [ ] Linux support (using V4L2 or similar)
- [ ] macOS support (using AVFoundation)
- [ ] Video recording
- [ ] Camera frame capture
- [ ] Multi-camera simultaneous display
- [ ] Camera property presets/profiles

## License

[Your License Here]
