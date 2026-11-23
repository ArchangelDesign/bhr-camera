# VS Code Debugging Guide

## Setup Complete ✅

Your VS Code debugging environment is now configured with:
- ✅ Launch configuration (`.vscode/launch.json`)
- ✅ Build tasks (`.vscode/tasks.json`)
- ✅ C++ settings (`.vscode/settings.json`)

---

## Prerequisites

### 1. Install C/C++ Extension
1. Open VS Code
2. Press `Ctrl+Shift+X` (Extensions)
3. Search: `C/C++`
4. Click **Install** (by Microsoft)

### 2. Install CMake
**Windows:**
- Download: https://cmake.org/download/
- Choose: "Windows x86-64 ZIP" or "Installer"
- If ZIP: Extract and add to PATH
- If Installer: Run and check "Add to PATH"
- **Restart terminal after installing**

**Verify installation:**
```powershell
cmake --version
```

---

## How to Debug

### Step 1: Open Workspace
1. File → Open Folder
2. Select: `C:\Users\Garage\Documents\bhr-camera`

### Step 2: Set Breakpoints
1. Open any `.cpp` file (e.g., `src/MainFrame.cpp`)
2. Click on line number (left margin) to set breakpoint
3. Red dot = breakpoint set

### Step 3: Start Debugging
**Method A: Quick Start**
- Press `F5` to start debugging
- App builds, launches, and breaks at first breakpoint

**Method B: Debug Menu**
- Go to: Run → Start Debugging
- Or: Run → Run Without Debugging (Ctrl+F5)

### Step 4: Step Through Code
| Key | Action |
|-----|--------|
| `F10` | Step Over (next line) |
| `F11` | Step Into (enter function) |
| `Shift+F11` | Step Out (exit function) |
| `F5` | Continue |
| `Shift+F5` | Stop debugging |

### Step 5: Inspect Variables
Hover over any variable to see its value, or:
- Look at **Debug Console** (bottom panel)
- Look at **Variables** panel (left sidebar)
- Look at **Watch** panel (add specific variables)

---

## Useful Debugging Features

### View Call Stack
1. While debugging, look at **Call Stack** panel (left)
2. Click frames to jump to that location

### View Variables
1. **Local Variables** panel shows all locals in scope
2. **Watch** panel - type variable names to watch
3. Hover over code to see values

### Debug Console
1. Bottom panel shows output
2. Type variable names to inspect
3. Can evaluate expressions

### Breakpoint Types
1. **Line breakpoint** - Click line number (red dot)
2. **Conditional** - Right-click breakpoint → Edit
3. **Logpoint** - Right-click line → Add Logpoint (prints without stopping)

---

## Common Debugging Scenarios

### Scenario 1: "Debug my camera enumeration"
```cpp
// In src/CameraManager.cpp, set breakpoint in EnumerateCameras():
std::vector<CameraInfo> CameraManager::EnumerateCameras() {
    std::vector<CameraInfo> cameras;  ← Click here to set breakpoint
    
    IMFAttributes* attributes = nullptr;
    // ... step through to see what's happening
}
```
Then press F5 to debug, and step through with F10.

### Scenario 2: "Why isn't my camera found?"
1. Set breakpoint in `EnumerateCameras()`
2. Press F5 to debug
3. Step through each line (F10)
4. Watch the `cameras` vector grow
5. If empty, check Device Manager

### Scenario 3: "Debug the settings dialog"
1. Set breakpoint in `SettingsDialog::PopulateCameras()`
2. Debug (F5)
3. App launches with breakpoint hit
4. Step through to see dropdowns populate

---

## Build & Debug Workflow

### Full Build + Debug
```
1. Press Ctrl+Shift+B (builds with tasks.json)
   Or: Run → Run Build Task
2. Wait for build to complete
3. Press F5 to debug (using launch.json)
```

### Quick Rebuild
```
1. Make code changes
2. Press F5 (automatically rebuilds via preLaunchTask)
3. Debugger launches new build
```

---

## Troubleshooting

### "CMake is not found"
**Solution:** Install CMake and restart VS Code
```powershell
# Verify CMake is in PATH
cmake --version
```

### "Cannot find executable"
**Solution:** Check build succeeded
1. Run `build.bat` manually
2. Verify `build\Release\bhr-camera.exe` exists

### "MSVC compiler not found"
**Solution:** Install Visual Studio 2017+ with C++ tools
- Or install Visual Studio Build Tools
- Then restart VS Code

### "Debugger won't start"
**Solution:** Try these steps
1. Close any existing `bhr-camera.exe` processes
2. Run → Run Without Debugging (Ctrl+F5) first
3. Then try Debug (F5)

### "Breakpoints not working"
**Solution:** 
1. Ensure Debug build (not Release)
2. Modify `launch.json` to use Debug build:
   ```json
   "program": "${workspaceFolder}/build/Debug/bhr-camera.exe"
   ```
3. Rebuild for Debug configuration

---

## Advanced Debugging

### Add Watch Expression
1. Debug Console → Click **+**
2. Enter: variable name or expression
3. It will evaluate each step

### Conditional Breakpoints
1. Right-click breakpoint (red dot)
2. Select "Edit Breakpoint"
3. Enter condition: e.g., `cameraIdx > 0`

### Logpoints (Print without stopping)
1. Right-click line number
2. Select "Add Logpoint"
3. Type message: e.g., `"Camera count: {cameras.size()}"`

### Remote Debugging
For debugging on another machine:
1. Install Remote Debugging Tools
2. Configure `launch.json` with `"remote": true`
3. See VS Code documentation for details

---

## Tips & Tricks

### Quick Restart
- `Ctrl+Shift+F5` restarts debugging (rebuilds if needed)

### Modify Value During Debug
- Right-click variable in Variables panel
- Select "Set Value"
- Enter new value

### Copy Variable Value
- Right-click in Variables panel
- "Copy" (copies to clipboard)

### Jump to Line
- During debug, click line number to move execution there
- Or press Ctrl+G and enter line number

### View Memory
- Right-click variable → "View Memory"
- Shows memory dump at that address

---

## Performance Profiling

For performance debugging (CPU/memory):

### Method 1: Windows Task Manager
1. Run your app
2. Open Task Manager (Ctrl+Shift+Esc)
3. Find `bhr-camera.exe`
4. See CPU% and Memory

### Method 2: VS Code Debugger
1. Debug → Start Profiler
2. Run your code
3. Stop profiler
4. View results in Debug Console

### Method 3: Visual Studio Profiler
If you need advanced profiling:
1. Open solution in Visual Studio
2. Debug → Performance Profiler
3. Select CPU or Memory
4. Run and analyze

---

## Next Steps

1. **Install CMake** (if not already)
2. **Restart VS Code**
3. **Press F5** to build and debug
4. **Set breakpoint** in `src/MainFrame.cpp` line 1
5. **Step through** the code with F10
6. **Inspect variables** in Variables panel

---

## Example: Debug Camera Enumeration

Here's a step-by-step example:

```cpp
// In src/CameraManager.cpp, line ~50

std::vector<CameraInfo> CameraManager::EnumerateCameras() {
    std::vector<CameraInfo> cameras;  // 1. Click here for breakpoint
    
    IMFAttributes* attributes = nullptr;
    HRESULT hr = MFCreateAttributes(&attributes, 1);
    if (FAILED(hr)) return cameras;  // 2. F10 to step over
    
    // ... more code
}
```

**Debugging steps:**
1. Click line 50 (red dot appears)
2. Press F5 (builds and debugs)
3. App breaks at line 50
4. Press F10 to go to line 51
5. Hover over `cameras` to see empty vector
6. Press F10 again
7. Hover over `attributes` to see pointer value
8. Continue stepping with F10
9. Watch `cameras` grow as cameras are added

---

## Quick Reference

| Task | Action |
|------|--------|
| Start debug | F5 |
| Stop debug | Shift+F5 |
| Step over | F10 |
| Step into | F11 |
| Step out | Shift+F11 |
| Continue | F5 |
| Set breakpoint | Click line number |
| Toggle breakpoint | F9 on line |
| View variables | Left panel "Variables" |
| Watch expression | Left panel "Watch" +  |
| Debug console | Ctrl+Shift+Y |
| Restart | Ctrl+Shift+F5 |

---

## Support

If you get stuck:
1. Check troubleshooting section above
2. See VS Code C++ documentation: https://code.visualstudio.com/docs/languages/cpp
3. Check CMake installation: https://cmake.org/

---

**You're ready to debug! Press F5 to get started. 🚀**
