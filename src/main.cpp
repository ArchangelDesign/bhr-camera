#include <wx/wx.h>
#include "MainFrame.h"

class BHRCameraApp : public wxApp {
public:
    bool OnInit() override {
        MainFrame* frame = new MainFrame(nullptr);
        frame->Show(true);
        return true;
    }
};

IMPLEMENT_APP(BHRCameraApp)
