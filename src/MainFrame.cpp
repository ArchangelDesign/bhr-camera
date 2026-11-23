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
    videoPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxDefaultSize, wxBORDER_NONE);
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
    
    // Set initial panel size and position to match frame client area
    videoPanel->SetSize(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight());
    
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
    wxPoint mousePos = wxGetMousePosition();
    wxRect frameRect = GetRect();
    
    int edgeThreshold = 15;
    int diffTop = mousePos.y - frameRect.GetTop();
    int diffBottom = frameRect.GetBottom() - mousePos.y;
    int diffLeft = mousePos.x - frameRect.GetLeft();
    int diffRight = frameRect.GetRight() - mousePos.x;
    
    // Check if near edge for resizing
    resizeEdge = 0;
    if (diffTop < edgeThreshold && diffTop >= 0) resizeEdge |= 1;      // top
    if (diffRight < edgeThreshold && diffRight >= 0) resizeEdge |= 2;    // right
    if (diffBottom < edgeThreshold && diffBottom >= 0) resizeEdge |= 4;   // bottom
    if (diffLeft < edgeThreshold && diffLeft >= 0) resizeEdge |= 8;     // left
    
    if (resizeEdge) {
        isResizing = true;
        CaptureMouse();  // Capture mouse even when cursor leaves window
    } else {
        isDragging = true;
        dragStartPos = mousePos - GetPosition();
        CaptureMouse();  // Also capture for dragging
    }
    event.Skip();
}

void MainFrame::OnLeftMouseUp(wxMouseEvent& event) {
    if (HasCapture()) {
        ReleaseMouse();
    }
    isDragging = false;
    isResizing = false;
    resizeEdge = 0;
    event.Skip();
}

void MainFrame::OnMouseMove(wxMouseEvent& event) {
    wxPoint mousePos = wxGetMousePosition();
    
    if (isDragging && HasCapture()) {
        wxPoint newPos = mousePos - dragStartPos;
        Move(newPos);
    } else if (isResizing && HasCapture()) {
        wxRect frameRect = GetRect();
        int newX = frameRect.x;
        int newY = frameRect.y;
        int newW = frameRect.width;
        int newH = frameRect.height;
        
        if (resizeEdge & 1) {  // top
            int delta = mousePos.y - frameRect.y;
            newY += delta;
            newH -= delta;
        }
        if (resizeEdge & 2) {  // right
            newW = mousePos.x - frameRect.x;
        }
        if (resizeEdge & 4) {  // bottom
            newH = mousePos.y - frameRect.y;
        }
        if (resizeEdge & 8) {  // left
            int delta = mousePos.x - frameRect.x;
            newX += delta;
            newW -= delta;
        }
        
        // Minimum size
        if (newW < 200) newW = 200;
        if (newH < 150) newH = 150;
        
        SetSize(newX, newY, newW, newH);
    } else {
        // Show resize cursor when near edges
        wxRect frameRect = GetRect();
        int edgeThreshold = 15;
        int diffTop = mousePos.y - frameRect.GetTop();
        int diffBottom = frameRect.GetBottom() - mousePos.y;
        int diffLeft = mousePos.x - frameRect.GetLeft();
        int diffRight = frameRect.GetRight() - mousePos.x;
        
        int edge = 0;
        if (diffTop < edgeThreshold && diffTop >= 0) edge |= 1;
        if (diffRight < edgeThreshold && diffRight >= 0) edge |= 2;
        if (diffBottom < edgeThreshold && diffBottom >= 0) edge |= 4;
        if (diffLeft < edgeThreshold && diffLeft >= 0) edge |= 8;
        
        // Set appropriate cursor
        wxStockCursor cursor = wxCURSOR_DEFAULT;
        if ((edge & 1) && !(edge & 2) && !(edge & 8)) cursor = wxCURSOR_SIZENS;        // top only
        else if ((edge & 4) && !(edge & 2) && !(edge & 8)) cursor = wxCURSOR_SIZENS;   // bottom only
        else if ((edge & 2) && !(edge & 1) && !(edge & 4)) cursor = wxCURSOR_SIZEWE;   // right only
        else if ((edge & 8) && !(edge & 1) && !(edge & 4)) cursor = wxCURSOR_SIZEWE;   // left only
        else if ((edge & 1) && (edge & 2)) cursor = wxCURSOR_SIZENWSE;                 // top-right
        else if ((edge & 1) && (edge & 8)) cursor = wxCURSOR_SIZENESW;                 // top-left
        else if ((edge & 4) && (edge & 2)) cursor = wxCURSOR_SIZENESW;                 // bottom-right
        else if ((edge & 4) && (edge & 8)) cursor = wxCURSOR_SIZENWSE;                 // bottom-left
        
        SetCursor(wxCursor(cursor));
    }
    event.Skip();
}

void MainFrame::OnFrameSizeChanged(wxSizeEvent& event) {
    // Fill entire frame with video panel
    if (videoPanel) {
        videoPanel->SetSize(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight());
    }
    event.Skip();
}

void MainFrame::OnPaint(wxPaintEvent& event) {
    if (event.GetEventObject() == videoPanel) {
        wxPaintDC dc(videoPanel);
        wxSize panelSize = videoPanel->GetSize();
        
        if (currentFrame.IsOk()) {
            if (keepAspectRatio) {
                // Check if frame has actually changed
                bool frameChanged = (lastCachedBitmap.GetRefData() != currentFrame.GetRefData());
                
                // Check if we need to regenerate the cached scaled frame
                bool needsRescale = !cachedScaledFrame.IsOk() || 
                                   frameChanged ||
                                   cachedWindowSize != panelSize;
                
                if (needsRescale) {
                    // Calculate scaling dimensions
                    int frameWidth = currentFrame.GetWidth();
                    int frameHeight = currentFrame.GetHeight();
                    int panelWidth = panelSize.GetWidth();
                    int panelHeight = panelSize.GetHeight();
                    
                    // Ensure valid dimensions
                    if (panelWidth <= 0 || panelHeight <= 0) {
                        return;
                    }
                    
                    double scaleX = (double)panelWidth / frameWidth;
                    double scaleY = (double)panelHeight / frameHeight;
                    double scale = wxMin(scaleX, scaleY);
                    
                    int scaledWidth = (int)(frameWidth * scale);
                    int scaledHeight = (int)(frameHeight * scale);
                    int offsetX = (panelWidth - scaledWidth) / 2;
                    int offsetY = (panelHeight - scaledHeight) / 2;
                    
                    // Create cached bitmap with black background and scaled frame
                    wxBitmap fullBitmap(panelWidth, panelHeight);
                    wxMemoryDC memDC;
                    memDC.SelectObject(fullBitmap);
                    
                    // Fill entire bitmap with black first
                    memDC.SetBrush(wxBrush(wxColour(0, 0, 0)));
                    memDC.SetPen(*wxTRANSPARENT_PEN);
                    memDC.DrawRectangle(0, 0, panelWidth, panelHeight);
                    
                    // Draw scaled frame centered
                    wxImage frameImg = currentFrame.ConvertToImage();
                    frameImg.Rescale(scaledWidth, scaledHeight, wxIMAGE_QUALITY_HIGH);
                    wxBitmap scaledBitmap = wxBitmap(frameImg);
                    if (scaledBitmap.IsOk()) {
                        memDC.DrawBitmap(scaledBitmap, offsetX, offsetY, false);
                    }
                    
                    memDC.SelectObject(wxNullBitmap);
                    
                    cachedScaledFrame = fullBitmap;
                    lastCachedBitmap = currentFrame;
                    cachedWindowSize = panelSize;
                }
                
                // Draw the cached scaled frame
                if (cachedScaledFrame.IsOk()) {
                    dc.DrawBitmap(cachedScaledFrame, 0, 0, false);
                }
            } else {
                // Stretch to fill entire panel
                if (currentFrame.GetWidth() == panelSize.GetWidth() && 
                    currentFrame.GetHeight() == panelSize.GetHeight()) {
                    dc.DrawBitmap(currentFrame, 0, 0, false);
                } else {
                    wxImage img = currentFrame.ConvertToImage();
                    img.Rescale(panelSize.GetWidth(), panelSize.GetHeight(), wxIMAGE_QUALITY_HIGH);
                    dc.DrawBitmap(wxBitmap(img), 0, 0, false);
                }
            }
            
            // Draw watermark in bottom right
            wxString watermark = "Developed by Black Horse Repairs LLC";
            wxFont watermarkFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            dc.SetFont(watermarkFont);
            dc.SetTextForeground(wxColour(200, 200, 200));
            wxSize textSize = dc.GetTextExtent(watermark);
            int watermarkX = panelSize.GetWidth() - textSize.GetWidth() - 25;
            int watermarkY = panelSize.GetHeight() - textSize.GetHeight() - 10;
            dc.DrawText(watermark, watermarkX, watermarkY);
            
            // Draw subtle resize handle in bottom-right corner
            int handleSize = 15;
            int handleX = panelSize.GetWidth() - handleSize;
            int handleY = panelSize.GetHeight() - handleSize;
            
            // Draw diagonal lines forming resize indicator (like a corner grip)
            dc.SetPen(wxPen(wxColour(100, 100, 100), 1));
            for (int i = 0; i < 3; i++) {
                int offset = i * 4;
                dc.DrawLine(handleX + offset, handleY + handleSize, handleX + handleSize, handleY + offset);
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
