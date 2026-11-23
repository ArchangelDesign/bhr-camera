cmake_minimum_required(VERSION 3.10)
project(BHRCamera)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to find wxWidgets with find_package  
find_package(wxWidgets COMPONENTS core base QUIET)

if(NOT wxWidgets_FOUND)
    # Manually specify wxWidgets libraries and paths
    message(STATUS "find_package(wxWidgets) failed, using manual configuration...")
    set(wxWidgets_FOUND TRUE)
    set(wxWidgets_INCLUDE_DIRS 
        "C:/wxWidgets/include"
        "C:/wxWidgets/lib/vc_x64_lib/mswu"
    )
    # Link ALL wxwidgets libraries to ensure everything is found
    set(wxWidgets_LIBRARIES 
        "C:/wxWidgets/lib/vc_x64_lib/wxmsw32u_core.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxbase32u.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxmsw32u_adv.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxmsw32u_aui.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxmsw32u_html.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxmsw32u_media.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxmsw32u_xrc.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxmsw32u_qa.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxmsw32u_ribbon.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxmsw32u_richtext.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxbase32u_xml.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxbase32u_net.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxpng.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxtiff.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxjpeg.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxzlib.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxregexu.lib"
        "C:/wxWidgets/lib/vc_x64_lib/wxexpat.lib"
    )
    set(wxWidgets_LIBRARY_DIRS "C:/wxWidgets/lib/vc_x64_lib")
    set(wxWidgets_CXX_FLAGS "/D__WXMSW__ /D_UNICODE /D_WINDOWS")
else()
    include(${wxWidgets_USE_FILE})
endif()

if(NOT wxWidgets_FOUND)
    message(FATAL_ERROR "wxWidgets not found")
endif()

message(STATUS "wxWidgets include dirs: ${wxWidgets_INCLUDE_DIRS}")
message(STATUS "wxWidgets libraries: ${wxWidgets_LIBRARIES}")

# Main executable
add_executable(bhr-camera
    src/main.cpp
    src/MainFrame.cpp
    src/MainFrame.h
    src/SettingsDialog.cpp
    src/SettingsDialog.h
    src/CameraManager.cpp
    src/CameraManager.h
)

# Add wxWidgets include directories
target_include_directories(bhr-camera PRIVATE ${wxWidgets_INCLUDE_DIRS})

# Add wxWidgets definitions
target_compile_options(bhr-camera PRIVATE ${wxWidgets_CXX_FLAGS})
target_compile_options(bhr-camera PRIVATE /D_UNICODE /D_CRT_SECURE_NO_WARNINGS /MD /D__WXMSW__ /DWIN32 /D_WINDOWS)

# Use dynamic runtime library like wxWidgets
set_property(TARGET bhr-camera PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

target_link_libraries(bhr-camera ${wxWidgets_LIBRARIES} 
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64/Mf.lib"
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64/Mfplat.lib"
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64/Mfreadwrite.lib"
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64/mfuuid.lib"
    strmiids
    kernel32 user32 gdi32 gdiplus msimg32 comdlg32 winspool winmm shell32 shlwapi comctl32 ole32 oleaut32 uuid rpcrt4 advapi32 version ws2_32 wininet oleacc uxtheme wsock32)

# Set linker flags for Windows subsystem and optimization
if(MSVC)
    target_compile_options(bhr-camera PRIVATE /O2 /GL)
    target_link_options(bhr-camera PRIVATE /LTCG /SUBSYSTEM:WINDOWS)
else()
    target_compile_options(bhr-camera PRIVATE -O2)
endif()
