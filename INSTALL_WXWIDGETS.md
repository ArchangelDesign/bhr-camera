# Installing wxWidgets

The build script is now detecting Visual Studio correctly! Now you need to install wxWidgets.

## Quick Steps

1. **Download wxWidgets**
   - Go to: https://wxwidgets.org/downloads/
   - Download "Source code (ZIP)" for Windows
   - Version 3.2.x or later recommended

2. **Extract to C:\wxWidgets**
   - Unzip the downloaded file
   - Move/rename the folder to: `C:\wxWidgets`
   - You should have: `C:\wxWidgets\include`, `C:\wxWidgets\src`, etc.

3. **Build wxWidgets**
   - Open Command Prompt
   - Navigate to: `C:\wxWidgets\build\msw`
   - Run: `nmake -f makefile.vc BUILD=release`
   - This will take 5-10 minutes

   Alternatively, you can use the pre-built binaries if available.

4. **Run build.bat again**
   - In the camera project folder, run: `build.bat`
   - Everything should work now!

## Detailed Steps (If Needed)

### Step 1: Download

```
1. Go to: https://wxwidgets.org/downloads/
2. Click "Source code (ZIP)" 
3. Wait for download (file is ~30 MB)
4. Extract the ZIP file
```

### Step 2: Extract

```
1. Right-click ZIP file → "Extract All..."
2. Extract to: C:\wxWidgets
3. Verify these folders exist:
   - C:\wxWidgets\include
   - C:\wxWidgets\src
   - C:\wxWidgets\build
```

### Step 3: Build wxWidgets

This is the longest step (5-10 minutes).

#### Option A: Using Command Prompt

```
1. Open Command Prompt (cmd.exe)
2. Navigate to wxWidgets folder:
   cd C:\wxWidgets\build\msw
3. Build:
   nmake -f makefile.vc BUILD=release
4. Wait for build to complete
```

#### Option B: Using Visual Studio IDE

```
1. Open Visual Studio
2. File → Open → Project/Solution
3. Navigate to: C:\wxWidgets\build\msw
4. Open: wx.sln
5. Build → Build Solution
6. Select "Release" configuration
7. Wait for build to complete
```

### Step 4: Verify

Check that these files exist:
```
C:\wxWidgets\lib\vc_x64_lib\wxmsw32u.lib
C:\wxWidgets\lib\vc_x64_lib\wxbase32u.lib
```

### Step 5: Build Camera App

Now run the build script:
```
build.bat
```

It should work!

## Troubleshooting

### "wxWidgets not found"

**Check:**
- Folder is at: `C:\wxWidgets` (not `C:\wxWidgets-3.2.4` or similar)
- Folder contains: `include`, `src`, `build` subdirectories

**Solution:**
- Rename the folder to exactly `wxWidgets`
- Or edit `build.bat` and change:
  ```batch
  set "WXWIDGETS_ROOT=C:\wxWidgets-3.2.4"
  ```

### Build fails with "include not found"

**Check:**
- wxWidgets was extracted correctly
- File paths in `build.bat` are correct:
  ```batch
  set "WXWIDGETS_LIB=C:\wxWidgets\lib\vc_x64_lib"
  ```

### nmake command not found

**Solution:**
- Use Visual Studio IDE instead (Option B above)
- Or open "Visual Studio Command Prompt" and try again

### Still having issues?

1. Check that wxWidgets is in `C:\wxWidgets`
2. Check that it was built successfully
3. Check `build\CMake.log` for specific errors
4. Try building wxWidgets again with more verbose output

---

**After installing wxWidgets, just run:**

```
build.bat
```

And the camera app will build! 🚀
