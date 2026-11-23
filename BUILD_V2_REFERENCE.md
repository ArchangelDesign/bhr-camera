# build.bat v2.0 - What's New Reference

## Summary

Your `build.bat` has been completely rewritten to be **foolproof and user-friendly**.

---

## 🎯 Main Features

| Feature | Benefit |
|---------|---------|
| **CMake Auto-Install** | No manual downloads needed |
| **Dependency Checking** | Catches problems early |
| **Clear Progress** | Knows what's happening |
| **Error Guidance** | Easy troubleshooting |
| **Full Logs** | For diagnostics |
| **Professional UX** | Polished output |

---

## 🚀 Usage

```bash
build.bat
```

That's it! The script handles everything.

---

## ⚡ What Happens

```
Step 1: Check CMake
   → Missing? Ask → Download → Install → Verify

Step 2: Check Visual Studio
   → Missing? Error + guidance → Exit
   → Found? Continue

Step 3: Check wxWidgets  
   → Missing? Error + guidance → Exit
   → Found? Continue

Step 4: Build
   → Error? Show logs → Exit
   → Success? Done!
```

---

## 🎁 When CMake is Missing

Script asks:
```
Would you like to download and install CMake now? (Y/N):
```

Type `Y` and it:
- Downloads automatically
- Launches installer
- Guides you through it
- Continues building

---

## 📊 Output Example

```
════════════════════════════════════════════════════════════════════════════════
                        BHR Camera Build Script v2.0
════════════════════════════════════════════════════════════════════════════════

[1/4] Checking prerequisites...

   • Checking for CMake...
     ✓ CMake found: 3.28.3
   • Checking for Visual Studio...
     ✓ Visual Studio Build Tools found
   • Checking for wxWidgets...
     ✓ wxWidgets found at: C:\wxWidgets

[2/4] Preparing build directory...

   • Build directory already exists
   • Cleaning old build files...
     ✓ Build directory cleaned

[3/4] Configuring with CMake...

     ✓ CMake configuration successful

[4/4] Building project...

     ✓ Build completed successfully!

════════════════════════════════════════════════════════════════════════════════
                              ✓ BUILD SUCCESSFUL!
════════════════════════════════════════════════════════════════════════════════

   Executable: C:\Users\Garage\Documents\bhr-camera\build\Release\bhr-camera.exe

   Next steps:
     1. Launch the app: build\Release\bhr-camera.exe
     2. Or open in VS Code for debugging: Press Ctrl+Shift+D
     3. Set breakpoints and press F5 to debug
```

---

## ✅ Error Handling

If something fails:

```
✗ CMake configuration failed!

Last 20 lines of CMake.log:
────────────────────────────────────────
error: wxWidgets not found at C:\wxWidgets
  [See full log in: build\CMake.log]
```

Script tells you:
- What failed
- Where to find full logs
- What to do next

---

## 🛠️ Configuration

Edit top of `build.bat`:

```batch
set "WXWIDGETS_ROOT=C:\wxWidgets"           # wxWidgets path
set "WXWIDGETS_LIB=C:\wxWidgets\lib\vc_x64_lib"
set "CMAKE_GENERATOR=Visual Studio 17 2022" # VS version
set "PLATFORM=x64"                          # x64 or x86
set "CMAKE_VERSION=3.28.3"                  # CMake version
```

---

## 📝 Logs Generated

After running:

```
build/
├── CMake.log       # CMake configuration output
└── Build.log       # Compiler output
```

Use for troubleshooting.

---

## 🎓 For Developers

### Customize CMake Version

Edit line ~16:
```batch
set "CMAKE_VERSION=3.29.0"
```

### Customize VS Generator

Edit line ~12:
```batch
set "CMAKE_GENERATOR=Visual Studio 16 2019"
```

### Customize Architecture

Edit line ~13:
```batch
set "PLATFORM=x86"
```

---

## 🔄 Rebuild

Just run again:
```
build.bat
```

Script automatically:
- Detects existing build
- Cleans old files
- Reconfigures
- Rebuilds only changed files

---

## 🚨 If Something Goes Wrong

1. **Read error message** - Usually tells you what to do
2. **Check logs** - Full details in `build/CMake.log` or `build/Build.log`
3. **Fix issue** - Install missing dependency
4. **Run again** - `build.bat`

---

## ✨ What Makes it Foolproof

✅ **Automatic CMake installation** - No manual downloads  
✅ **Clear error messages** - Knows what went wrong  
✅ **Actionable guidance** - Shows next steps  
✅ **Full logs** - For advanced troubleshooting  
✅ **Dependency checking** - Catches problems early  
✅ **Professional output** - Polished appearance  
✅ **User-friendly** - Easy to use even for first-timers  

---

## 🎯 Typical Workflow

```
Day 1: First build
  $ build.bat
  → CMake not found
  → Type Y to install
  → Follow installer
  → Build succeeds
  → Done!

Day 2+: Rebuild
  $ build.bat
  → All checks pass
  → Build complete
  → Done!
```

---

## 📚 More Info

- **BUILD_UPDATE_SUMMARY.md** - Complete update details
- **BUILD_SCRIPT_GUIDE.md** - Comprehensive guide  
- **BUILD_QUICK_START.md** - Quick reference

---

## 🏁 TL;DR

Old build.bat:
```
❌ CMake missing = Error
❌ Vague error messages
❌ Manual troubleshooting
```

New build.bat v2.0:
```
✅ CMake missing = Auto-install
✅ Clear error messages
✅ Automatic troubleshooting
✅ Foolproof!
```

---

**Just run `build.bat` and it handles everything!** 🚀
