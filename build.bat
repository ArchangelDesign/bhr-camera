@echo off
REM BHR Camera Build Script - Simple Edition
REM This script checks for dependencies and builds the project

setlocal enabledelayedexpansion

REM ============================================================================
REM CONFIGURATION
REM ============================================================================

set "WXWIDGETS_ROOT=C:\wxWidgets"
set "WXWIDGETS_LIB=C:\wxWidgets\lib\vc_x64_lib"
REM Use NMake Makefiles generator which uses the vcvars environment we set up
set "CMAKE_GENERATOR=NMake Makefiles"
set "PLATFORM=x64"

REM ============================================================================
REM DISPLAY HEADER
REM ============================================================================

cls
echo.
echo ════════════════════════════════════════════════════════════════════════════════
echo                      BHR Camera Build Script
echo ════════════════════════════════════════════════════════════════════════════════
echo.

REM ============================================================================
REM CHECK PREREQUISITES
REM ============================================================================

echo [1/4] Checking prerequisites...
echo.

REM Check for CMake
echo   • Checking for CMake...
where cmake >nul 2>&1
if errorlevel 1 (
    echo     ✗ CMake not found
    echo.
    echo     Install CMake from: https://cmake.org/download/
    echo     Make sure to check "Add CMake to system PATH" during installation
    echo.
    pause
    exit /b 1
) else (
    for /f "tokens=3" %%i in ('cmake --version ^| findstr /R "^cmake"') do set "CMAKE_FOUND=%%i"
    echo     ✓ CMake found: !CMAKE_FOUND!
)

REM Check for Visual Studio Build Tools
echo   • Checking for Visual Studio...
set "VS_FOUND=0"
where cl.exe >nul 2>&1
if errorlevel 1 (
    REM cl.exe not in PATH, try to find Visual Studio installation
    if exist "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC" (
        set "VS_FOUND=1"
        echo     ✓ Visual Studio found
    ) else if exist "C:\Program Files\Microsoft Visual Studio\17\Community\VC\Tools\MSVC" (
        set "VS_FOUND=1"
        echo     ✓ Visual Studio found
    ) else if exist "C:\Program Files\Microsoft Visual Studio\18\Professional\VC\Tools\MSVC" (
        set "VS_FOUND=1"
        echo     ✓ Visual Studio found
    ) else if exist "C:\Program Files\Microsoft Visual Studio\17\Professional\VC\Tools\MSVC" (
        set "VS_FOUND=1"
        echo     ✓ Visual Studio found
    ) else if exist "C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\MSVC" (
        set "VS_FOUND=1"
        echo     ✓ Visual Studio found
    ) else if exist "C:\Program Files\Microsoft Visual Studio\17\Enterprise\VC\Tools\MSVC" (
        set "VS_FOUND=1"
        echo     ✓ Visual Studio found
    )
    
    if !VS_FOUND! equ 0 (
        echo     ✗ Visual Studio/Build Tools not found
        echo.
        echo     Install from: https://visualstudio.microsoft.com/
        echo     Make sure to select "Desktop development with C++"
        echo.
        pause
        exit /b 1
    )
    
    REM Detect Visual Studio version for CMake generator
    REM Note: VS 2024 (version 18) works with Visual Studio 17 2022 generator
    if exist "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC" (
        set "CMAKE_GENERATOR=NMake Makefiles"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\18\Professional\VC\Tools\MSVC" (
        set "CMAKE_GENERATOR=NMake Makefiles"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\MSVC" (
        set "CMAKE_GENERATOR=NMake Makefiles"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\17\Community\VC\Tools\MSVC" (
        set "CMAKE_GENERATOR=NMake Makefiles"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\17\Professional\VC\Tools\MSVC" (
        set "CMAKE_GENERATOR=NMake Makefiles"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\17\Enterprise\VC\Tools\MSVC" (
        set "CMAKE_GENERATOR=NMake Makefiles"
    )
) else (
    echo     ✓ Visual Studio Build Tools found in PATH
)

REM Check for wxWidgets and its libraries
echo   • Checking for wxWidgets...
set "WXWIDGETS_BUILT=0"
if exist "!WXWIDGETS_ROOT!" (
    REM Check if libs exist
    if exist "!WXWIDGETS_LIB!\wxmsw*.lib" (
        set "WXWIDGETS_BUILT=1"
        echo     ✓ wxWidgets found and built at: !WXWIDGETS_ROOT!
    ) else (
        echo     ⚠ wxWidgets folder found but needs to be built
        call :setup_wxwidgets
        if errorlevel 1 (
            echo.
            echo ✗ wxWidgets setup failed. Exiting.
            pause
            exit /b 1
        )
    )
) else (
    echo     ✗ wxWidgets not found at: !WXWIDGETS_ROOT!
    echo.
    call :setup_wxwidgets
    if errorlevel 1 (
        echo.
        echo ✗ wxWidgets setup failed. Exiting.
        pause
        exit /b 1
    )
)

echo.
echo ════════════════════════════════════════════════════════════════════════════════
echo.

REM ============================================================================
REM PREPARE BUILD
REM ============================================================================

echo [2/4] Preparing build directory...

if not exist "build" (
    echo   • Creating build directory...
    mkdir build
    echo     ✓ Build directory created
) else (
    echo   • Build directory already exists
    echo   • Cleaning old build files...
    cd build
    if exist "CMakeCache.txt" del CMakeCache.txt
    if exist "CMakeFiles" rmdir /s /q CMakeFiles >nul 2>&1
    cd ..
    echo     ✓ Build directory cleaned
)

echo.

REM ============================================================================
REM CONFIGURE WITH CMAKE
REM ============================================================================

REM Set up Visual Studio x64 build environment (required for cl.exe and x64 compilation)
echo [3/4] Setting up build environment...
set "VS_DEVENV_FOUND=0"

if exist "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsx86_amd64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsx86_amd64.bat" >nul 2>&1
    echo   ✓ Visual Studio 2024 Community x64 environment ready
    set "VS_DEVENV_FOUND=1"
) else if exist "C:\Program Files\Microsoft Visual Studio\17\Community\VC\Auxiliary\Build\vcvarsx86_amd64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\17\Community\VC\Auxiliary\Build\vcvarsx86_amd64.bat" >nul 2>&1
    echo   ✓ Visual Studio 2022 Community x64 environment ready
    set "VS_DEVENV_FOUND=1"
) else if exist "C:\Program Files\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvarsx86_amd64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvarsx86_amd64.bat" >nul 2>&1
    echo   ✓ Visual Studio 2024 Professional x64 environment ready
    set "VS_DEVENV_FOUND=1"
) else if exist "C:\Program Files\Microsoft Visual Studio\17\Professional\VC\Auxiliary\Build\vcvarsx86_amd64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\17\Professional\VC\Auxiliary\Build\vcvarsx86_amd64.bat" >nul 2>&1
    echo   ✓ Visual Studio 2022 Professional x64 environment ready
    set "VS_DEVENV_FOUND=1"
)

if !VS_DEVENV_FOUND! equ 0 (
    echo   ⚠ Could not set up Visual Studio environment
    echo   This may cause build failures
)

echo.
echo [4/4] Configuring with CMake...
echo.

cd build

echo   Running: cmake -G "!CMAKE_GENERATOR!" ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DwxWidgets_ROOT_DIR="!WXWIDGETS_ROOT!" ^
  -DwxWidgets_LIB_DIR="!WXWIDGETS_LIB!" ..
echo.

cmake -G "!CMAKE_GENERATOR!" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DwxWidgets_ROOT_DIR="!WXWIDGETS_ROOT!" ^
    -DwxWidgets_LIB_DIR="!WXWIDGETS_LIB!" ^
    .. >CMake.log 2>&1

if errorlevel 1 (
    echo.
    echo   ✗ CMake configuration failed!
    echo.
    echo   Last 20 lines of CMake.log:
    echo   ────────────────────────────────────────
    type CMake.log | findstr /R "^" | <nul findstr /R "^" ^
        && (for /f "skip=999" %%a in (CMake.log) do set "last=%%a") ^
        || (for /f %%a in (CMake.log) do set "last=%%a")
    echo     [See full log in: build\CMake.log]
    echo.
    pause
    cd ..
    exit /b 1
)

echo     ✓ CMake configuration successful
echo.

REM ============================================================================
REM BUILD PROJECT
REM ============================================================================

echo [4/4] Building project...
echo.

REM Build using cmake (works with both NMake and Visual Studio generators)
cmake --build . --config Release >Build.log 2>&1

if errorlevel 1 (
    echo.
    echo   ✗ Build failed!
    echo.
    echo   Last 20 lines of Build.log:
    echo   ────────────────────────────────────────
    type Build.log | findstr /R "^" | <nul findstr /R "^" ^
        && (for /f "skip=999" %%a in (Build.log) do set "last=%%a") ^
        || (for /f %%a in (Build.log) do set "last=%%a")
    echo     [See full log in: build\Build.log]
    echo.
    pause
    cd ..
    exit /b 1
)

echo     ✓ Build completed successfully!
echo.

REM ============================================================================
REM SUCCESS - SHOW RESULTS
REM ============================================================================

cd ..

echo ════════════════════════════════════════════════════════════════════════════════
echo                              ✓ BUILD SUCCESSFUL!
echo ════════════════════════════════════════════════════════════════════════════════
echo.
echo   Executable: %CD%\build\bhr-camera.exe
echo.
echo   Next steps:
echo     1. Launch the app: build\bhr-camera.exe
echo     2. Or open in VS Code for debugging: Press Ctrl+Shift+D
echo     3. Set breakpoints and press F5 to debug
echo.
echo ════════════════════════════════════════════════════════════════════════════════
echo.

pause
exit /b 0

REM ============================================================================
REM SUBROUTINES
REM ============================================================================

:setup_wxwidgets
REM Download, extract, and build wxWidgets
echo.
echo   wxWidgets is required but not installed.
echo   Downloading and building wxWidgets automatically...
echo   (This will take 5-10 minutes)
echo.

goto install_wxwidgets

:install_wxwidgets
echo   Downloading wxWidgets 3.2.4 (~30 MB)...
echo.

REM Download wxWidgets using PowerShell
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
    "try { $ProgressPreference = 'SilentlyContinue'; Invoke-WebRequest -Uri 'https://github.com/wxWidgets/wxWidgets/releases/download/v3.2.4/wxWidgets-3.2.4.zip' -OutFile '%TEMP%\wxWidgets.zip' -TimeoutSec 300; Write-Host '   ✓ Download complete' } catch { Write-Host '   ✗ Download failed: ' $_.Exception.Message; exit 1 }"

if errorlevel 1 (
    echo.
    echo   ✗ Failed to download wxWidgets
    echo.
    exit /b 1
)

echo.
echo   Extracting wxWidgets...
echo.

REM Clean up old wxWidgets folder if it exists
if exist "C:\wxWidgets" (
    echo   Removing old wxWidgets folder...
    rmdir /s /q "C:\wxWidgets" >nul 2>&1
)

REM Clean up temp extraction directory
if exist "%TEMP%\wx-extract" (
    rmdir /s /q "%TEMP%\wx-extract" >nul 2>&1
)

REM Extract to temp directory first (ZIP has no containing folder)
mkdir "%TEMP%\wx-extract" >nul 2>&1
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
    "try { Expand-Archive -Path '%TEMP%\wxWidgets.zip' -DestinationPath '%TEMP%\wx-extract' -Force -ErrorAction Stop; Write-Host '   ✓ Extraction complete' } catch { Write-Host '   ✗ Extraction failed: ' $_.Exception.Message; exit 1 }"

if errorlevel 1 (
    echo.
    echo   ✗ Failed to extract wxWidgets
    echo.
    exit /b 1
)

REM Move from temp to final location
if exist "%TEMP%\wx-extract" (
    echo   Moving to final location...
    move "%TEMP%\wx-extract" "C:\wxWidgets" >nul 2>&1
    if errorlevel 1 (
        echo.
        echo   ✗ Failed to move wxWidgets to C:\wxWidgets
        echo.
        exit /b 1
    )
)

echo.
echo   Building wxWidgets (this will take 10-15 minutes)...
echo.

REM Save current directory
set "PROJECT_DIR=%CD%"

REM Set up Visual Studio build environment for nmake (x64)
if exist "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" (
    call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 >nul 2>&1
) else if exist "C:\Program Files\Microsoft Visual Studio\17\Community\Common7\Tools\VsDevCmd.bat" (
    call "C:\Program Files\Microsoft Visual Studio\17\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 >nul 2>&1
)

REM Also call the x64 vcvars to ensure proper environment
if exist "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsx86_amd64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsx86_amd64.bat" >nul 2>&1
)

REM Build wxWidgets using nmake (normal multi-library build for 64-bit)
cd /d "C:\wxWidgets\build\msw"
nmake -f makefile.vc BUILD=release TARGET_CPU=X64

if errorlevel 1 (
    echo.
    echo   ✗ wxWidgets build failed
    echo.
    echo   To try building wxWidgets manually, run:
    echo   ────────────────────────────────────────────────────────────────
    echo   call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsx86_amd64.bat" ^&^& cd /d C:\wxWidgets\build\msw ^&^& nmake -f makefile.vc BUILD=release TARGET_CPU=X64
    echo   ────────────────────────────────────────────────────────────────
    echo.
    cd /d "!PROJECT_DIR!"
    exit /b 1
)

echo.
echo   ✓ wxWidgets built successfully!
cd /d "!PROJECT_DIR!"
echo.
exit /b 0

endlocal
