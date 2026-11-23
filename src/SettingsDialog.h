#pragma once

#include <wx/wx.h>

class CameraManager;

class SettingsDialog : public wxDialog {
public:
    SettingsDialog(wxWindow* parent, CameraManager* cameraManager);
    
private:
    CameraManager* cameraManager;
    wxComboBox* cameraChoice;
    wxComboBox* resolutionChoice;
    wxComboBox* framerateChoice;
    
    void OnCameraChanged(wxCommandEvent& event);
    void OnResolutionChanged(wxCommandEvent& event);
    void OnFramerateChanged(wxCommandEvent& event);
    void OnCameraSettings(wxCommandEvent& event);
    void OnOK(wxCommandEvent& event);
    
    void PopulateCameras();
    void PopulateResolutions();
    void PopulateFramerates();
    
    wxDECLARE_EVENT_TABLE();
};
