# 🎯 BHR Camera - Build & Run

## ⚡ Quickest Start

```bash
build.bat
```

The script checks for dependencies and builds the project.

---

## 📋 Before You Run It

You need three things installed:

### 1. CMake
- Download: https://cmake.org/download/
- Install Windows version
- **Important:** Check "Add CMake to system PATH" ✓

### 2. Visual Studio
- Download: https://visualstudio.microsoft.com/
- Install Community Edition (free)
- **Important:** Select "Desktop development with C++" ✓

### 3. wxWidgets
- Download: https://wxwidgets.org/downloads/
- Extract to: `C:\wxWidgets`

---

## 🚀 Run It

```bash
build.bat
```

The script will:
- ✓ Check if everything is installed
- ✓ Tell you what's missing (if anything)
- ✓ Build the project
- ✓ Show success or error details

---

## ✅ Success!

```
✓ BUILD SUCCESSFUL!
Executable: build\Release\bhr-camera.exe
```

Launch the app:
```bash
build\Release\bhr-camera.exe
```

Or debug in VS Code:
1. Press `Ctrl+Shift+D` (Debug panel)
2. Click "Run and Debug"
3. Press `F5` to start debugging

---

## ❓ What If It Fails?

The script tells you exactly what's wrong:

```
✗ CMake not found
Install CMake from: https://cmake.org/download/
```

**Follow the instructions and try again.**

For more details, check these files:
- `build\CMake.log` - CMake errors
- `build\Build.log` - Compilation errors

---

## 📚 Documentation

| File | Purpose |
|------|---------|
| **00_BUILD_QUICK_START_SIMPLE.md** | Step-by-step guide |
| **BUILD_SIMPLE_GUIDE.md** | Detailed reference |
| **BUILD_SIMPLIFICATION.md** | Why we simplified it |
| **README.md** | Project overview |

---

## 🔧 Customize (Optional)

Edit top of `build.bat` to change paths:

```batch
set "WXWIDGETS_ROOT=C:\wxWidgets"
set "CMAKE_GENERATOR=Visual Studio 17 2022"
```

---

## 🎓 Debugging

VS Code is pre-configured for debugging:

1. Open VS Code in project folder
2. Set a breakpoint by clicking left of line number
3. Press `F5` to build and debug
4. Step through code with keyboard:
   - `F10` - Step over
   - `F11` - Step into
   - `Shift+F11` - Step out

Detailed guide: See `.vscode/launch.json`

---

**Next:** Run `build.bat` → Follow instructions → Build succeeds! 🚀

Simple. Reliable. Works.
