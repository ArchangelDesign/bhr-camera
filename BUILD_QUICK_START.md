# build.bat v2.0 - Quick Reference

## One Command Build

```bash
build.bat
```

That's it! The script will:
- ✅ Check for CMake (install if missing)
- ✅ Check for Visual Studio
- ✅ Check for wxWidgets
- ✅ Configure project
- ✅ Build everything
- ✅ Show result

---

## What Happens

```
Run: build.bat
   ↓
[1/4] Check Prerequisites
   • CMake? (auto-install if missing) ✓
   • Visual Studio? (error if missing) ✓
   • wxWidgets? (error if missing) ✓
   ↓
[2/4] Prepare Build Directory
   • Create/clean build/ folder ✓
   ↓
[3/4] Configure with CMake
   • Generate Visual Studio files ✓
   ↓
[4/4] Build Project
   • Compile source code ✓
   ↓
SUCCESS: build\Release\bhr-camera.exe
```

---

## If CMake is Missing

Script will ask:
```
CMake is required but not installed.

Would you like to download and install CMake now?
(Y/N): 
```

Type **Y** and it will:
1. Download CMake automatically
2. Launch installer
3. Guide you through installation
4. Verify it worked
5. Continue building

---

## If Something Goes Wrong

The script will:
1. Show clear error message
2. Display last 20 lines of log
3. Tell you full log location
4. Provide next steps

**Example:**
```
✗ CMake configuration failed!

Last 20 lines of CMake.log:
────────────────────────────────────────
error: wxWidgets not found
  [See full log in: build\CMake.log]
```

---

## Customization

Edit top of `build.bat`:

```batch
set "WXWIDGETS_ROOT=C:\wxWidgets"           # wxWidgets path
set "WXWIDGETS_LIB=C:\wxWidgets\lib\vc_x64_lib"
set "CMAKE_GENERATOR=Visual Studio 17 2022" # VS version
set "PLATFORM=x64"                          # x64 or x86
set "CMAKE_VERSION=3.28.3"                  # CMake version
```

---

## Common Issues & Fixes

| Issue | Solution |
|-------|----------|
| CMake not found | Script auto-installs it |
| VS not found | Download from visualstudio.com |
| wxWidgets not found | Download & extract to C:\wxWidgets |
| Build fails | Check build\CMake.log or build\Build.log |
| Need to rebuild | Just run build.bat again |

---

## After Build Succeeds

```
Executable: build\Release\bhr-camera.exe

Next steps:
  1. Run it: build\Release\bhr-camera.exe
  2. Debug: Press Ctrl+Shift+D in VS Code
  3. Implement: See IMPLEMENTATION_NOTES.md
```

---

## Keyboard Shortcuts

| Key | Action |
|-----|--------|
| Ctrl+C | Cancel build anytime |
| Any key | Continue after error |

---

## Check Build Logs

```
build/CMake.log       ← CMake configuration details
build/Build.log       ← Compilation details
```

---

## For Manual Debug

If you want to debug the build process:

```bash
# Just run CMake manually
cd build
cmake -G "Visual Studio 17 2022" -A x64 ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DwxWidgets_ROOT_DIR="C:\wxWidgets" ^
  -DwxWidgets_LIB_DIR="C:\wxWidgets\lib\vc_x64_lib" ^
  ..

# Then build
cmake --build . --config Release
```

---

## Features

✅ Auto-detects missing dependencies  
✅ Auto-downloads & installs CMake  
✅ Guides you through installation  
✅ Clear progress indicators  
✅ Detailed error messages  
✅ Saves full build logs  
✅ Professional output formatting  
✅ Verifies everything worked  

---

## That's It!

Just run:
```
build.bat
```

And let it handle everything! 🚀

See `BUILD_SCRIPT_GUIDE.md` for comprehensive documentation.
