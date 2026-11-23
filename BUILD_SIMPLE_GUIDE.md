# Simple Build Script Guide

## Overview

The `build.bat` script has been simplified to focus on what it does best:
- **Detect** missing dependencies
- **Report** clearly where to get them
- **Build** the project

No automatic downloads or installations. Just clear, straightforward building.

## How It Works

### Step 1: Check Prerequisites

The script checks for all required tools:

```
[1/4] Checking prerequisites...
  • Checking for CMake...
  • Checking for Visual Studio...
  • Checking for wxWidgets...
```

If any tool is missing, the script will:
1. Tell you what's missing
2. Show you where to download it
3. Exit with a clear error message

### Step 2: Prepare Build Directory

```
[2/4] Preparing build directory...
  • Creating/cleaning build directory
```

### Step 3: Configure with CMake

```
[3/4] Configuring with CMake...
  • Running CMake to generate build files
```

### Step 4: Build Project

```
[4/4] Building project...
  • Compiling the application
```

## Usage

Just run:

```bash
build.bat
```

## What to Do If Something Is Missing

The script will tell you exactly what's missing and where to get it.

### If CMake is missing:

```
✗ CMake not found

Install CMake from: https://cmake.org/download/
Make sure to check "Add CMake to system PATH" during installation
```

**Action:** 
1. Download from: https://cmake.org/download/
2. Install it
3. **Important:** Check "Add CMake to system PATH" during installation
4. Restart your command prompt
5. Run `build.bat` again

### If Visual Studio is missing:

```
✗ Visual Studio/Build Tools not found

Install from: https://visualstudio.microsoft.com/
Make sure to select "Desktop development with C++"
```

**Action:**
1. Download from: https://visualstudio.microsoft.com/
2. Install Visual Studio or Build Tools
3. **Important:** Select "Desktop development with C++" workload
4. Run `build.bat` again

### If wxWidgets is missing:

```
✗ wxWidgets not found at: C:\wxWidgets

Install wxWidgets from: https://wxwidgets.org/downloads/
Extract to: C:\wxWidgets
```

**Action:**
1. Download wxWidgets from: https://wxwidgets.org/downloads/
2. Extract to `C:\wxWidgets`
3. Run `build.bat` again

## If Build Fails

If the build fails, the script will show you:

```
✗ CMake configuration failed!

Last 20 lines of CMake.log:
────────────────────────────────────────
[error details here]
[See full log in: build\CMake.log]
```

**To debug:**
1. Open `build\CMake.log` for the full error message
2. Search for the error online
3. Fix the issue
4. Run `build.bat` again

## Success

When everything works:

```
════════════════════════════════════════════════════════════════════════════════
                              ✓ BUILD SUCCESSFUL!
════════════════════════════════════════════════════════════════════════════════

   Executable: C:\...\bhr-camera\build\Release\bhr-camera.exe

   Next steps:
     1. Launch the app: build\Release\bhr-camera.exe
     2. Or open in VS Code for debugging: Press Ctrl+Shift+D
     3. Set breakpoints and press F5 to debug
```

## Logs

The script saves detailed logs:

- **build\CMake.log** - CMake configuration output
- **build\Build.log** - Compiler and linker output

Check these files if you need to debug build issues.

## Configuration

You can customize the script by editing the top section:

```batch
set "WXWIDGETS_ROOT=C:\wxWidgets"
set "WXWIDGETS_LIB=C:\wxWidgets\lib\vc_x64_lib"
set "CMAKE_GENERATOR=Visual Studio 17 2022"
set "PLATFORM=x64"
```

- **WXWIDGETS_ROOT**: Path to wxWidgets installation
- **WXWIDGETS_LIB**: Path to wxWidgets lib directory
- **CMAKE_GENERATOR**: Visual Studio version
- **PLATFORM**: Build platform (x64 or Win32)

## Troubleshooting

### "CMake not found"
- Install CMake from: https://cmake.org/download/
- Make sure to check "Add CMake to system PATH"
- Restart your command prompt

### "Visual Studio not found"
- Install Visual Studio from: https://visualstudio.microsoft.com/
- Make sure to select "Desktop development with C++"

### "wxWidgets not found"
- Download wxWidgets from: https://wxwidgets.org/downloads/
- Extract to: C:\wxWidgets

### Build still fails?
- Check **build\CMake.log** for CMake errors
- Check **build\Build.log** for compiler errors
- Make sure all prerequisites are installed correctly
- Try deleting the `build` directory and running `build.bat` again

## Questions?

Refer to the main documentation:
- **README.md** - Project overview
- **GETTING_STARTED.md** - Setup instructions
- **ARCHITECTURE.md** - Technical architecture

---

**Version:** Simple Edition  
**Status:** Production ready  
**Purpose:** Fast, reliable builds with clear error reporting
