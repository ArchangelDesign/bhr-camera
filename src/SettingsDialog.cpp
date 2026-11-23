#include "SettingsDialog.h"
#include "CameraManager.h"
#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
    EVT_COMBOBOX(wxID_ANY, SettingsDialog::OnCameraChanged)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent, CameraManager* cameraManager)
    : wxDialog(parent, wxID_ANY, "Camera Settings", wxDefaultPosition, wxSize(400, 350)),
      cameraManager(cameraManager),
      mainFrame(dynamic_cast<MainFrame*>(parent)) {
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
    // Camera selection
    wxBoxSizer* cameraSizer = new wxBoxSizer(wxHORIZONTAL);
    cameraSizer->Add(new wxStaticText(this, wxID_ANY, "Camera:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    cameraChoice = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, nullptr, wxCB_READONLY);
    cameraChoice->Bind(wxEVT_COMBOBOX, &SettingsDialog::OnCameraChanged, this);
    cameraSizer->Add(cameraChoice, 1, wxALL, 5);
    sizer->Add(cameraSizer, 0, wxEXPAND | wxALL, 5);
    
    // Resolution selection
    wxBoxSizer* resSizer = new wxBoxSizer(wxHORIZONTAL);
    resSizer->Add(new wxStaticText(this, wxID_ANY, "Resolution:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    resolutionChoice = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, nullptr, wxCB_READONLY);
    resolutionChoice->Bind(wxEVT_COMBOBOX, &SettingsDialog::OnResolutionChanged, this);
    resSizer->Add(resolutionChoice, 1, wxALL, 5);
    sizer->Add(resSizer, 0, wxEXPAND | wxALL, 5);
    
    // Framerate selection
    wxBoxSizer* fpsSizer = new wxBoxSizer(wxHORIZONTAL);
    fpsSizer->Add(new wxStaticText(this, wxID_ANY, "Framerate:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    framerateChoice = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, nullptr, wxCB_READONLY);
    framerateChoice->Bind(wxEVT_COMBOBOX, &SettingsDialog::OnFramerateChanged, this);
    fpsSizer->Add(framerateChoice, 1, wxALL, 5);
    sizer->Add(fpsSizer, 0, wxEXPAND | wxALL, 5);
    
    // Keep aspect ratio checkbox
    keepAspectRatioCheckbox = new wxCheckBox(this, wxID_ANY, "Keep aspect ratio");
    keepAspectRatioCheckbox->SetValue(mainFrame ? mainFrame->GetKeepAspectRatio() : true);
    sizer->Add(keepAspectRatioCheckbox, 0, wxALL, 10);
    
    // Camera settings button
    wxButton* cameraSettingsBtn = new wxButton(this, wxID_ANY, "Camera Settings...");
    cameraSettingsBtn->Bind(wxEVT_BUTTON, &SettingsDialog::OnCameraSettings, this);
    sizer->Add(cameraSettingsBtn, 0, wxEXPAND | wxALL, 5);
    
    // OK/Cancel buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okBtn = new wxButton(this, wxID_OK, "OK");
    wxButton* cancelBtn = new wxButton(this, wxID_CANCEL, "Cancel");
    okBtn->Bind(wxEVT_BUTTON, &SettingsDialog::OnOK, this);
    buttonSizer->Add(okBtn, 0, wxALL, 5);
    buttonSizer->Add(cancelBtn, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);
    
    SetSizer(sizer);
    
    PopulateCameras();
    PopulateResolutions();
    PopulateFramerates();
}

void SettingsDialog::PopulateCameras() {
    cameraChoice->Clear();
    auto cameras = cameraManager->EnumerateCameras();
    for (const auto& camera : cameras) {
        cameraChoice->Append(camera.name);
    }
    if (!cameras.empty()) {
        cameraChoice->SetSelection(0);
    }
}

void SettingsDialog::PopulateResolutions() {
    resolutionChoice->Clear();
    auto resolutions = cameraManager->GetSupportedResolutions();
    for (const auto& res : resolutions) {
        wxString label = wxString::Format("%dx%d", res.first, res.second);
        resolutionChoice->Append(label);
    }
    if (!resolutions.empty()) {
        resolutionChoice->SetSelection(0);
    }
}

void SettingsDialog::PopulateFramerates() {
    framerateChoice->Clear();
    auto framerates = cameraManager->GetSupportedFramerates();
    for (int fps : framerates) {
        framerateChoice->Append(wxString::Format("%d FPS", fps));
    }
    if (!framerates.empty()) {
        framerateChoice->SetSelection(0);
    }
}

void SettingsDialog::OnCameraChanged(wxCommandEvent& event) {
    PopulateResolutions();
    PopulateFramerates();
}

void SettingsDialog::OnResolutionChanged(wxCommandEvent& event) {
    // Resolution changed
}

void SettingsDialog::OnFramerateChanged(wxCommandEvent& event) {
    // Framerate changed
}

void SettingsDialog::OnCameraSettings(wxCommandEvent& event) {
    wxMessageBox("Camera settings dialog not yet implemented.\n"
                 "This will open the camera driver's settings dialog.",
                 "Camera Settings", wxOK | wxICON_INFORMATION);
}

void SettingsDialog::OnOK(wxCommandEvent& event) {
    int cameraIdx = cameraChoice->GetSelection();
    int resIdx = resolutionChoice->GetSelection();
    int fpsIdx = framerateChoice->GetSelection();
    
    if (cameraIdx != wxNOT_FOUND && resIdx != wxNOT_FOUND && fpsIdx != wxNOT_FOUND) {
        auto cameras = cameraManager->EnumerateCameras();
        auto resolutions = cameraManager->GetSupportedResolutions();
        auto framerates = cameraManager->GetSupportedFramerates();
        
        if (cameraIdx < cameras.size() && resIdx < resolutions.size() && fpsIdx < framerates.size()) {
            cameraManager->InitializeCamera(
                cameras[cameraIdx].deviceId,
                resolutions[resIdx].first,
                resolutions[resIdx].second,
                framerates[fpsIdx]
            );
        }
    }
    
    // Update aspect ratio setting
    if (mainFrame) {
        mainFrame->SetKeepAspectRatio(keepAspectRatioCheckbox->GetValue());
        mainFrame->Refresh(); // Redraw with new setting
    }
    
    EndModal(wxID_OK);
}
