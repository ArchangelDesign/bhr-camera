#pragma once

#include <wx/wx.h>
#include <memory>

class CameraManager;

class MainFrame : public wxFrame {
public:
    MainFrame(wxWindow* parent);
    ~MainFrame();
    
    bool GetKeepAspectRatio() const { return keepAspectRatio; }
    void SetKeepAspectRatio(bool keep) { keepAspectRatio = keep; cachedFrameSize = wxSize(-1, -1); }

private:
    std::unique_ptr<CameraManager> cameraManager;
    wxPanel* videoPanel;
    wxBitmap currentFrame;
    wxBitmap cachedScaledFrame;
    wxSize cachedFrameSize;
    wxSize cachedWindowSize;
    wxBitmap lastCachedBitmap;  // Track if frame actually changed
    wxTimer* timer = nullptr;
    
    // Settings
    bool keepAspectRatio = true;
    
    // Dragging state
    bool isDragging = false;
    bool isResizing = false;
    int resizeEdge = 0;  // 1=top, 2=right, 4=bottom, 8=left, combinations for corners
    wxPoint dragStartPos;
    
    void OnDoubleClick(wxMouseEvent& event);
    void OnFrameSizeChanged(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnLeftMouseDown(wxMouseEvent& event);
    void OnLeftMouseUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnTimer(wxTimerEvent& event);
    
    void RenderFrame();
    
    wxDECLARE_EVENT_TABLE();
};
