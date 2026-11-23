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
    
    SetBackgroundColour(*wxBLACK);
    
    // Create panel to hold video - fill entire client area
    videoPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
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
    
    // Set initial panel size to match frame client area
    videoPanel->SetSize(GetClientSize());
    
    // Start timer to update frames
    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);
    timer = new wxTimer(this);
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
    if (event.GetEventObject() == videoPanel) {
        wxPaintDC dc(videoPanel);
        wxSize panelSize = videoPanel->GetSize();
        
        if (currentFrame.IsOk()) {
            // We have a real frame - display it
            if (currentFrame.GetWidth() == panelSize.GetWidth() && 
                currentFrame.GetHeight() == panelSize.GetHeight()) {
                // Perfect size - just draw it
                dc.DrawBitmap(currentFrame, 0, 0, false);
            } else {
                // Need to scale - rescale the frame to fit
                wxImage img = currentFrame.ConvertToImage();
                img.Rescale(panelSize.GetWidth(), panelSize.GetHeight(), wxIMAGE_QUALITY_HIGH);
                dc.DrawBitmap(wxBitmap(img), 0, 0, false);
            }
        } else {
            // No frame yet - draw test pattern so we know something is happening
            dc.SetBrush(*wxBLACK_BRUSH);
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(0, 0, panelSize.GetWidth(), panelSize.GetHeight());
            
            // Draw colored grid pattern to show UI is responsive
            dc.SetPen(wxPen(wxColour(50, 50, 50), 1));
            int gridSize = 50;
            for (int x = 0; x < panelSize.GetWidth(); x += gridSize) {
                dc.DrawLine(x, 0, x, panelSize.GetHeight());
            }
            for (int y = 0; y < panelSize.GetHeight(); y += gridSize) {
                dc.DrawLine(0, y, panelSize.GetWidth(), y);
            }
            
            // Draw status text in center
            dc.SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            dc.SetTextForeground(wxColour(0, 200, 0));
            wxString status = "Waiting for camera...";
            wxSize textSize = dc.GetTextExtent(status);
            int x = (panelSize.GetWidth() - textSize.GetWidth()) / 2;
            int y = (panelSize.GetHeight() - textSize.GetHeight()) / 2;
            dc.DrawText(status, x, y);
        }
    } else {
        event.Skip();
    }
}

void MainFrame::OnTimer(wxTimerEvent& event) {
    RenderFrame();
}

void MainFrame::RenderFrame() {
    // Try multiple times to get a frame since the first attempts might fail
    // as the camera warms up or buffers data
    static int frameAttempts = 0;
    
    wxBitmap frame = cameraManager->GetCurrentFrame();
    if (frame.IsOk()) {
        currentFrame = frame;
        frameAttempts = 0; // Reset on success
    } else {
        frameAttempts++;
        // Keep trying - don't give up easily
        if (frameAttempts > 100) {
            // After many attempts without success, something might be wrong
            // but we'll keep trying anyway
            frameAttempts = 50; // Reset to prevent int overflow
        }
    }
    
    // Always request paint update to show current state
    if (videoPanel) {
        videoPanel->Refresh(false);
    }
}
