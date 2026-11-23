# VS Code Debugging Setup - Quick Start

## ✅ What's Been Added

Three configuration files for VS Code debugging:

1. **`.vscode/launch.json`** - Debugging configuration
2. **`.vscode/tasks.json`** - Build task (runs build.bat before debugging)
3. **`.vscode/settings.json`** - C++ compiler and intellisense settings

---

## 🚀 Quick Start (5 Steps)

### Step 1: Install C/C++ Extension (2 min)
```
1. Open VS Code
2. Press Ctrl+Shift+X (Extensions)
3. Search: "C/C++"
4. Install (by Microsoft)
```

### Step 2: Install CMake (10 min)
```
1. Download: https://cmake.org/download/
2. Choose: Windows x86-64 ZIP or Installer
3. If Installer: Run and check "Add to PATH"
4. Restart terminal/VS Code
5. Verify: cmake --version
```

### Step 3: Open Workspace
```
1. File → Open Folder
2. Select: C:\Users\Garage\Documents\bhr-camera
```

### Step 4: Set a Breakpoint
```
1. Open: src/MainFrame.cpp
2. Click line number (e.g., line 15)
3. Red dot appears = breakpoint set
```

### Step 5: Start Debugging
```
Press F5 to:
  • Build the project
  • Launch the app
  • Stop at your breakpoint
  • Step through code with F10
```

---

## 🎯 Debugging Commands

| Shortcut | Action |
|----------|--------|
| **F5** | Start/Continue debugging |
| **F10** | Step Over (next line) |
| **F11** | Step Into (enter function) |
| **Shift+F11** | Step Out (exit function) |
| **Shift+F5** | Stop debugging |
| **Ctrl+Shift+F5** | Restart debugging |

---

## 📊 Debug Views (Left Panel)

When debugging (F5), you'll see:

- **Variables** - All local variables and their values
- **Watch** - Add variables to monitor
- **Call Stack** - Trace function calls
- **Breakpoints** - Manage all breakpoints
- **Debug Console** - Output and expressions

---

## 🔧 Before First Debug

**Checklist:**
- [ ] Install C/C++ extension
- [ ] Install CMake
- [ ] Run `cmake --version` in terminal
- [ ] Restart VS Code
- [ ] Open folder in VS Code
- [ ] Ready to press F5

---

## ⚠️ If CMake Error Persists

If you still get "cmake is not recognized":

1. **Install CMake properly:**
   - Download installer: https://cmake.org/download/
   - Run installer
   - **CHECK: "Add CMake to system PATH"**
   - Click Install
   - Restart ALL terminals and VS Code

2. **Verify installation:**
   ```powershell
   # Open NEW PowerShell/CMD
   cmake --version
   ```

3. **If still not found:**
   - Edit `build.bat`
   - Replace first line with full path:
     ```batch
     "C:\Program Files\CMake\bin\cmake" -G "Visual Studio 17 2022" ...
     ```

---

## 📖 More Help

- **Full guide:** `DEBUGGING_GUIDE.md` (comprehensive)
- **Build guide:** `README.md`
- **Getting started:** `GETTING_STARTED.md`

---

## 🎓 Example Debug Session

Here's what happens when you press F5:

```
1. VS Code reads launch.json
2. Runs build task (build.bat)
3. CMake generates Visual Studio files
4. Visual Studio builds the project
5. Executable created: build\Release\bhr-camera.exe
6. Debugger starts
7. App launches
8. Breaks at first breakpoint
9. You can step through code
```

---

## 🎊 Ready?

1. Install extensions & CMake (follow Quick Start)
2. Open workspace in VS Code
3. Set breakpoint (click line number)
4. Press **F5**
5. Watch code execute line-by-line!

---

**Next: See DEBUGGING_GUIDE.md for comprehensive debugging help**
