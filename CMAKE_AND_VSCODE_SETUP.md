# Complete VS Code + CMake Setup Guide

## 🎯 Your Situation

You tried to build but got:
```
'cmake' is not recognized as an internal or external command
```

This is solved with one installation.

---

## ✅ What's Been Set Up

### 1. CMake Error - Solution
**You need:** CMake installed and in PATH

**Fix (10 minutes):**
1. Go to: https://cmake.org/download/
2. Download: "Windows x86-64 Installer" (blue button)
3. Run the installer
4. **IMPORTANT:** Check "Add CMake to system PATH"
5. Click Install
6. **Close all terminals and restart them**
7. Verify: Open PowerShell, type `cmake --version`

### 2. VS Code Debugging - Complete Setup
**Created 3 configuration files:**

| File | Purpose |
|------|---------|
| `.vscode/launch.json` | Tells VS Code how to debug your app |
| `.vscode/tasks.json` | Tells VS Code to build before debugging |
| `.vscode/settings.json` | C++ compiler and IntelliSense settings |

---

## 🚀 To Use Debugging in VS Code

### Prerequisites (Do These First)
1. ✅ Install CMake (see above)
2. ✅ Install **C/C++ extension** in VS Code (Ctrl+Shift+X, search "C/C++")
3. ✅ Restart VS Code

### How to Debug
1. **Open workspace in VS Code:**
   - File → Open Folder
   - Select: `C:\Users\Garage\Documents\bhr-camera`

2. **Set a breakpoint:**
   - Open any `.cpp` file (e.g., `src/MainFrame.cpp`)
   - Click on a line number (left side)
   - Red dot appears = breakpoint set

3. **Start debugging:**
   - Press `F5`
   - VS Code automatically:
     - Builds your project
     - Launches the application
     - Breaks at your breakpoint

4. **Step through code:**
   - `F10` = Next line
   - `F11` = Step into function
   - `Shift+F11` = Step out of function
   - `F5` = Continue running

---

## 📚 Documentation Added

| File | Purpose |
|------|---------|
| `DEBUGGING_GUIDE.md` | Complete debugging reference (8 KB) |
| `VSCODE_SETUP_QUICK.md` | Quick reference card (3 KB) |

---

## 🎓 Example Debug Session

**Goal:** See why cameras aren't being found

1. Open: `src/CameraManager.cpp`
2. Find: `EnumerateCameras()` function
3. Click line 52 (first line in function)
   - Red dot appears
4. Press `F5`
   - Build starts
   - App launches
   - Breaks at line 52
5. Press `F10` repeatedly
   - Each press = next line
   - Watch `cameras` vector on left panel
   - See how many cameras are found
6. When done, press `Shift+F5` to stop

---

## 🛠️ Debug Views When Running (F5)

**Left Panel (when debugging):**
- 🔵 **Variables** - All local variables and their current values
- 👁️ **Watch** - Add specific variables to monitor
- 📞 **Call Stack** - Function trace (what called what)
- 🔴 **Breakpoints** - List of all breakpoints

**Bottom Panel:**
- **Debug Console** - Program output and you can evaluate expressions

**Main Window:**
- Current line highlighted in yellow
- Hover over variables to see their values

---

## ⌨️ Keyboard Shortcuts

```
F5                   Start or continue debugging
F10                  Step over (execute current line, skip functions)
F11                  Step into (enter function being called)
Shift+F11            Step out (finish current function)
Shift+F5             Stop debugging
Ctrl+Shift+F5        Restart (rebuild and debug)
Ctrl+Shift+Y         Open Debug Console
Ctrl+B               Toggle sidebar
```

---

## ⚠️ Troubleshooting

### "CMake still not found"
```
1. Download installer: https://cmake.org/download/
2. Run installer
3. ** MUST CHECK: "Add CMake to system PATH" **
4. Click Install
5. CLOSE ALL TERMINALS (including VS Code terminal)
6. Restart terminals
7. Test: cmake --version
```

### "C/C++ extension issues"
```
1. Ctrl+Shift+X (Extensions)
2. Search: C/C++
3. Click "Install" on the official one (by Microsoft)
4. Reload VS Code (Ctrl+Shift+P → Reload Window)
```

### "Build fails"
```
1. Ensure Visual Studio 2017+ is installed with C++ tools
2. Or install Visual Studio Build Tools
3. Restart VS Code
4. Try F5 again
```

### "Breakpoints don't stop"
```
1. Build succeeded? (run build.bat first)
2. Restart VS Code
3. Try F5 again
4. Check launch.json is valid JSON (no typos)
```

---

## 🎁 You Now Have

✅ **CMake debugging path guide** (this doc)
✅ **Comprehensive debugging guide** (DEBUGGING_GUIDE.md)
✅ **Quick reference** (VSCODE_SETUP_QUICK.md)
✅ **VS Code config files** (.vscode folder with 3 files)
✅ **Ready-to-debug setup** (just install CMake!)

---

## 🚦 Next Steps

1. **Install CMake** (10 min) - https://cmake.org/download/
   - Download installer
   - Run it
   - Check "Add to PATH"
   - Restart terminal/VS Code

2. **Install C/C++ extension** (1 min)
   - Ctrl+Shift+X
   - Search "C/C++"
   - Install

3. **Test debugging** (5 min)
   - Press F5 in VS Code
   - See app build and launch
   - Set a breakpoint and step through

---

## 📖 More Help

| Need | Read |
|------|------|
| Detailed debugging help | `DEBUGGING_GUIDE.md` |
| Quick reference | `VSCODE_SETUP_QUICK.md` |
| Build instructions | `README.md` |
| Getting started | `GETTING_STARTED.md` |

---

## ✨ What Makes This Awesome

✅ **One-command build & debug** - Just press F5  
✅ **Step line-by-line** - F10 to execute one line at a time  
✅ **See variables** - Watch panel shows all variable values  
✅ **Inspect expressions** - Hover over code to see values  
✅ **Conditional breakpoints** - Only break when condition is true  
✅ **Call stack** - See entire function trace  

---

## 🎊 Ready to Debug?

After CMake is installed:

1. Ctrl+Shift+X → Install "C/C++"
2. Restart VS Code
3. Press F5
4. Debugging works! 🎉

---

**Key point:** The most important step is installing CMake with PATH enabled. After that, everything else "just works" with the configuration I've created.

If you have any issues after installing CMake, see the **Troubleshooting** section above.
