#pragma once

#include <wx/wx.h>
#include <memory>

class CameraManager;

class MainFrame : public wxFrame {
public:
    MainFrame(wxWindow* parent);
    ~MainFrame();

private:
    std::unique_ptr<CameraManager> cameraManager;
    wxPanel* videoPanel;
    wxBitmap currentFrame;
    
    // Dragging state
    bool isDragging = false;
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
