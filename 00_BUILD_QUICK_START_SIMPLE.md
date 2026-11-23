# 🚀 Quick Start - Build Script (Simplified Edition)

## Just Run This

```bash
build.bat
```

That's it! The script will:
1. Check if you have everything needed
2. Tell you what to install if something's missing
3. Build the project if everything's ready

## Typical Flow

### First Time (Missing Tools)

```
$ build.bat

[1/4] Checking prerequisites...
  ✗ CMake not found
  
  Install CMake from: https://cmake.org/download/
  Make sure to check "Add CMake to system PATH" during installation
```

**What to do:**
1. Go to https://cmake.org/download/
2. Download CMake
3. Run installer
4. **CHECK: "Add CMake to system PATH"** ← Important!
5. Restart command prompt
6. Run `build.bat` again

### After Installing Everything

```
$ build.bat

[1/4] Checking prerequisites...
  ✓ CMake found: 3.28.3
  ✓ Visual Studio Build Tools found
  ✓ wxWidgets found at: C:\wxWidgets

[2/4] Preparing build directory...
  ✓ Build directory cleaned

[3/4] Configuring with CMake...
  ✓ CMake configuration successful

[4/4] Building project...
  ✓ Build completed successfully!

════════════════════════════════════════════════════════════════════════════════
                              ✓ BUILD SUCCESSFUL!
════════════════════════════════════════════════════════════════════════════════

   Executable: C:\Users\...\bhr-camera\build\Release\bhr-camera.exe
   
   Next steps:
     1. Launch: build\Release\bhr-camera.exe
     2. Debug in VS Code: Press Ctrl+Shift+D then F5
```

## Prerequisites Checklist

Before running `build.bat`, you need:

- [ ] **CMake** - https://cmake.org/download/
  - Download for Windows
  - Run installer
  - **Check: "Add CMake to system PATH"**

- [ ] **Visual Studio** - https://visualstudio.microsoft.com/
  - Download Community Edition (free)
  - Run installer
  - **Select: "Desktop development with C++"**
  - Install C++ workload

- [ ] **wxWidgets** - https://wxwidgets.org/downloads/
  - Download source code
  - Extract to `C:\wxWidgets`

## What the Script Does

| Step | What | Status |
|------|------|--------|
| 1 | Check CMake installed | ✓ Detects |
| 2 | Check Visual Studio installed | ✓ Detects |
| 3 | Check wxWidgets present | ✓ Detects |
| 4 | Create build folder | ✓ Automatic |
| 5 | Configure with CMake | ✓ Automatic |
| 6 | Compile project | ✓ Automatic |
| 7 | Report success/errors | ✓ Automatic |

## If Something Is Wrong

The script will tell you exactly what's missing and where to get it.

```
✗ Visual Studio/Build Tools not found

Install from: https://visualstudio.microsoft.com/
Make sure to select "Desktop development with C++"
```

**To fix:** Follow the instructions and try again.

## Logs for Debugging

If the build fails, check these files:

- `build\CMake.log` - CMake configuration errors
- `build\Build.log` - Compilation errors

Open them in any text editor to see what went wrong.

## Success!

```
✓ BUILD SUCCESSFUL!

Executable: C:\...\bhr-camera\build\Release\bhr-camera.exe

Next steps:
  1. Launch the app: build\Release\bhr-camera.exe
  2. Or debug in VS Code: Press Ctrl+Shift+D then F5
```

## In VS Code

To debug with breakpoints:

1. Open **VS Code** in the project folder
2. Press `Ctrl+Shift+D` (Debug panel)
3. Click **"Run and Debug"** or press `F5`
4. Script builds, then launches debugger
5. Set breakpoints, step through code

## Common Issues

| Problem | Solution |
|---------|----------|
| "CMake not found" | Install from https://cmake.org/download/ and check "Add to PATH" |
| "cl.exe not found" | Install Visual Studio with C++ workload |
| "wxWidgets not found" | Extract wxWidgets to C:\wxWidgets |
| Build still fails | Check build\CMake.log for details |

## Customize (Optional)

Edit the top of `build.bat` to change paths:

```batch
set "WXWIDGETS_ROOT=C:\wxWidgets"
set "CMAKE_GENERATOR=Visual Studio 17 2022"
set "PLATFORM=x64"
```

Default settings work for most Windows PCs.

## Questions?

Read these docs:

- **BUILD_SIMPLE_GUIDE.md** - Detailed guide
- **BUILD_SIMPLIFICATION.md** - Why we simplified it
- **README.md** - Project overview

---

**TL;DR:** Run `build.bat` → Follow instructions → Success!

Simple. Reliable. Fast.
