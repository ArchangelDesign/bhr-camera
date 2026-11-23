# CMake & VS Code Debugging - Complete Solution

## 🎯 Your Two Issues - SOLVED

### Issue 1: CMake Not Found ❌ → ✅ SOLVED
**Problem:** `'cmake' is not recognized as an internal or external command`

**Solution:** Download & install CMake
- **Go to:** https://cmake.org/download/
- **Download:** Windows x86-64 Installer
- **Install:** Run installer, CHECK "Add CMake to system PATH"
- **Restart:** Close all terminals and VS Code
- **Verify:** Type `cmake --version` in new terminal

---

### Issue 2: Want to Debug in VS Code? ✅ COMPLETE
**Answer:** Yes! Full debugging setup is installed

**What you get:**
- ✅ Set breakpoints by clicking line numbers
- ✅ Step through code with F10/F11
- ✅ Watch variables and their values
- ✅ See call stack and execution trace
- ✅ Debug Console for expressions
- ✅ Automatic build before debugging

**How to use:**
1. Install C/C++ extension (Ctrl+Shift+X → search "C/C++")
2. Click line number to set breakpoint (red dot)
3. Press F5 to build and debug
4. Step through with F10

---

## 📦 What's Been Created

### Configuration Files
```
.vscode/
├── launch.json      (Debugging configuration)
├── tasks.json       (Build task automation)
└── settings.json    (C++ IntelliSense settings)
```

These files tell VS Code:
- How to build your project (runs build.bat)
- How to debug your project (uses MSVC debugger)
- How to analyze your C++ code (IntelliSense)

### Documentation Files
```
CMAKE_AND_VSCODE_SETUP.md    (This comprehensive guide)
DEBUGGING_GUIDE.md            (Detailed debugging reference)
VSCODE_SETUP_QUICK.md         (Quick reference card)
```

---

## 🚀 Quick Start (3 Steps)

### Step 1: Install CMake (10 min)
```
1. Go to: https://cmake.org/download/
2. Download: "Windows x86-64 Installer" (blue button)
3. Run installer
4. ⚠️ IMPORTANT: CHECK "Add CMake to system PATH"
5. Click Install
6. Restart ALL terminals and VS Code
7. Test: Open PowerShell, type cmake --version
```

### Step 2: Install C/C++ Extension (1 min)
```
1. Open VS Code
2. Press Ctrl+Shift+X (Extensions)
3. Search: C/C++
4. Click Install (by Microsoft)
```

### Step 3: Debug Your Code (Anytime)
```
1. File → Open Folder → Select your project
2. Open any .cpp file
3. Click line number to set breakpoint (red dot)
4. Press F5 to build and debug
5. Press F10 to step through line-by-line
6. Watch variables in left panel
7. Press Shift+F5 to stop
```

---

## ⌨️ Debugging Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| **F5** | Start/Continue debugging |
| **F10** | Step Over (next line, don't enter functions) |
| **F11** | Step Into (enter function being called) |
| **Shift+F11** | Step Out (exit current function) |
| **Shift+F5** | Stop debugging |
| **Ctrl+Shift+F5** | Restart (rebuild and debug) |
| **Ctrl+Shift+Y** | Open Debug Console |

---

## 👀 Debug View (When F5 is Running)

**Left Panel:**
- 🔵 **Variables** - All local variables and their values
- 👁️ **Watch** - Add variables to monitor
- 📞 **Call Stack** - Function trace
- 🔴 **Breakpoints** - All breakpoints

**Bottom Panel:**
- **Debug Console** - Program output and expression evaluation

**Main Editor:**
- Yellow highlight on current line
- Hover over variables to see values

---

## 🎓 Example: Debug Camera Enumeration

**Goal:** See how many cameras are found and debug why

**Steps:**
1. Open: `src/CameraManager.cpp`
2. Find: `EnumerateCameras()` function
3. Click line 52 (first line in function)
   - Red dot appears = breakpoint set
4. Press `F5`
   - Build starts
   - App launches
   - Breaks at line 52
5. Press `F10` to step through each line
   - Watch left panel "Variables"
   - See `cameras` vector grow
   - Hover over `hr` to see return codes
6. When done, press `Shift+F5`

**What you learned:**
- ✅ How many cameras were found
- ✅ What return codes were returned
- ✅ Exactly where the code breaks (if it does)
- ✅ All variable values at each step

---

## ⚠️ Troubleshooting

### "CMake Still Not Found"
```
1. Download from: https://cmake.org/download/
2. Choose: Windows x86-64 Installer (NOT ZIP)
3. Run the installer
4. ⚠️ MUST CHECK: "Add CMake to system PATH"
5. Click Install
6. CLOSE ALL terminals and VS Code
7. Open NEW terminal and type: cmake --version
```

### "C/C++ Extension Not Working"
```
1. Ctrl+Shift+X (Extensions)
2. Search: C/C++
3. Make sure the official one (by Microsoft) is installed
4. If installed, click "Uninstall"
5. Reload VS Code (Ctrl+Shift+P → Reload Window)
6. Search again and Install
```

### "Build Fails When Pressing F5"
```
1. Make sure Visual Studio 2017+ is installed with C++ tools
   OR Visual Studio Build Tools
2. Restart VS Code
3. Try F5 again
4. Check launch.json for syntax errors (should be valid JSON)
```

### "Breakpoints Don't Stop Execution"
```
1. Build first: run build.bat manually
2. Verify it completes successfully
3. Restart VS Code
4. Try F5 again
5. If still not working, check .vscode/launch.json is valid
```

### "Debugger Won't Launch"
```
1. Close any existing bhr-camera.exe processes
2. Try Ctrl+F5 (Run Without Debugging) first
3. If that works, try F5 (Debug)
4. Check VS Code terminal for error messages
```

---

## 📚 Documentation Guide

| File | Use For | Read Time |
|------|---------|-----------|
| **CMAKE_AND_VSCODE_SETUP.md** (this) | Complete setup guide | 10 min |
| **DEBUGGING_GUIDE.md** | Comprehensive debugging reference | 15 min |
| **VSCODE_SETUP_QUICK.md** | Quick reference card | 3 min |
| **README.md** | Build & project setup | 10 min |
| **GETTING_STARTED.md** | Complete getting started | 20 min |

---

## 🎁 Why This Setup is Awesome

✅ **One Command Build & Debug** - Press F5 and everything happens automatically  
✅ **Line-by-Line Stepping** - F10 executes one line at a time  
✅ **Variable Inspection** - Hover code or use left panel to see values  
✅ **Expression Evaluation** - Right-click to edit values during debugging  
✅ **Call Stack Trace** - See entire function call chain  
✅ **Conditional Breakpoints** - Only break when condition is true  
✅ **Breakpoint Logging** - Print without stopping (logpoints)  
✅ **Memory Inspection** - View raw memory at addresses  

---

## 🔄 Typical Debug Workflow

```
1. Write code
   ↓
2. Set breakpoint (click line number)
   ↓
3. Press F5 (build + debug)
   ↓
4. App hits breakpoint (stops at that line)
   ↓
5. Press F10 to step through line-by-line
   ↓
6. Watch variables on left panel
   ↓
7. Hover over code to see values
   ↓
8. Find the bug!
   ↓
9. Press Shift+F5 to stop
   ↓
10. Fix the code
   ↓
11. Repeat from step 2
```

---

## ✨ What Makes Debugging Powerful

**Without Debugging:**
- Add print statements
- Recompile
- Run program
- Check output
- Modify code
- Repeat 10+ times

**With Debugging (F5 → F10 → hover):**
- Set breakpoint
- Run once
- Step through
- Watch variables
- Find bug immediately

---

## 🎯 Next Steps

1. **Install CMake** (Most Important!)
   - Download: https://cmake.org/download/
   - Install with "Add to PATH"
   - Restart terminal/VS Code

2. **Install C/C++ Extension**
   - Ctrl+Shift+X → Search "C/C++" → Install

3. **Test Build & Debug**
   - Open project in VS Code
   - Set breakpoint in any .cpp file
   - Press F5

4. **Happy Debugging! 🎉**

---

## 📞 Quick Reference

**Most Important:**
- 🟢 F5 = Start debugging (builds and runs)
- 🟡 F10 = Step over next line
- 🔴 Shift+F5 = Stop debugging

**Setup Checklist:**
- [ ] CMake installed (cmake --version works)
- [ ] C/C++ extension installed (Ctrl+Shift+X)
- [ ] VS Code pointed at project (File → Open Folder)
- [ ] .vscode folder exists with 3 files

**You're Ready When:**
- [ ] CMake installed and PATH updated
- [ ] C/C++ extension installed
- [ ] Can set breakpoint (click line number)
- [ ] Can press F5 to build and debug

---

## 🏁 Summary

**Problem:** CMake not found  
**Solution:** Install from https://cmake.org/download/

**Question:** Can I debug in VS Code?  
**Answer:** Yes! Already set up. Just press F5.

**Before First Debug:**
1. Install CMake
2. Install C/C++ extension
3. Restart VS Code

**Then:**
1. Set breakpoint (click line)
2. Press F5 to debug
3. Use F10 to step through

**You're all set! 🚀**

---

*Created: Complete debugging setup for BHR Camera project*  
*Status: Ready to debug once CMake is installed*  
*Next Step: Install CMake from https://cmake.org/download/*
