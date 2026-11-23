#include "MainFrame.h"
#include "CameraManager.h"
#include "SettingsDialog.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_LEFT_DCLICK(MainFrame::OnDoubleClick)
    EVT_SIZE(MainFrame::OnFrameSizeChanged)
    EVT_PAINT(MainFrame::OnPaint)
    EVT_LEFT_DOWN(MainFrame::OnLeftMouseDown)
    EVT_LEFT_UP(MainFrame::OnLeftMouseUp)
    EVT_MOTION(MainFrame::OnMouseMove)
    EVT_TIMER(wxID_ANY, MainFrame::OnTimer)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "BHR Camera", wxDefaultPosition, wxSize(800, 600),
              wxFRAME_NO_TASKBAR | wxNO_BORDER | wxSTAY_ON_TOP) {
    
    // Create panel to hold video
    videoPanel = new wxPanel(this);
    videoPanel->SetBackgroundColour(*wxBLACK);
    
    // Bind events to both frame and panel for dragging
    Bind(wxEVT_LEFT_DOWN, &MainFrame::OnLeftMouseDown, this);
    Bind(wxEVT_LEFT_UP, &MainFrame::OnLeftMouseUp, this);
    Bind(wxEVT_MOTION, &MainFrame::OnMouseMove, this);
    
    // Bind double-click event to panel
    videoPanel->Bind(wxEVT_LEFT_DCLICK, &MainFrame::OnDoubleClick, this);
    videoPanel->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnLeftMouseDown, this);
    videoPanel->Bind(wxEVT_LEFT_UP, &MainFrame::OnLeftMouseUp, this);
    videoPanel->Bind(wxEVT_MOTION, &MainFrame::OnMouseMove, this);
    videoPanel->Bind(wxEVT_PAINT, &MainFrame::OnPaint, this);
    
    // Initialize camera manager
    cameraManager = std::make_unique<CameraManager>();
    
    // Start streaming from first camera
    if (!cameraManager->InitializeFirstCamera()) {
        wxLogError("Failed to initialize camera");
    }
    
    // Start timer to update frames
    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);
    wxTimer* timer = new wxTimer(this);
    timer->Start(33); // ~30 FPS
}

MainFrame::~MainFrame() {
    if (cameraManager) {
        cameraManager->Stop();
    }
}

void MainFrame::OnDoubleClick(wxMouseEvent& event) {
    SettingsDialog dialog(this, cameraManager.get());
    if (dialog.ShowModal() == wxID_OK) {
        // Settings were applied, camera manager will handle updates
    }
}

void MainFrame::OnLeftMouseDown(wxMouseEvent& event) {
    isDragging = true;
    dragStartPos = wxGetMousePosition() - GetPosition();
    event.Skip();
}

void MainFrame::OnLeftMouseUp(wxMouseEvent& event) {
    isDragging = false;
    event.Skip();
}

void MainFrame::OnMouseMove(wxMouseEvent& event) {
    if (isDragging) {
        wxPoint mousePos = wxGetMousePosition();
        wxPoint newPos = mousePos - dragStartPos;
        Move(newPos);
    }
    event.Skip();
}

void MainFrame::OnFrameSizeChanged(wxSizeEvent& event) {
    // Fill entire frame with video panel
    videoPanel->SetSize(GetClientSize());
    event.Skip();
}

void MainFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(videoPanel);
    if (!currentFrame.IsOk()) {
        dc.SetBrush(*wxBLACK_BRUSH);
        dc.DrawRectangle(0, 0, videoPanel->GetSize().GetWidth(), videoPanel->GetSize().GetHeight());
    } else {
        dc.DrawBitmap(currentFrame, 0, 0, false);
    }
}

void MainFrame::OnTimer(wxTimerEvent& event) {
    RenderFrame();
}

void MainFrame::RenderFrame() {
    // Get frame from camera manager
    wxBitmap frame = cameraManager->GetCurrentFrame();
    if (frame.IsOk()) {
        currentFrame = frame;
        // Request paint event
        if (videoPanel) {
            videoPanel->Refresh(false);
        }
    }
}
