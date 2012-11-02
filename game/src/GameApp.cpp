#include <wx/app.h>
#include <wx/image.h>
#include <wx/log.h>

#include "GameFrame.h"

class GameApp : public wxApp {

public:

    bool OnInit() {

        wxInitAllImageHandlers();

        wxLog::EnableLogging(false);

        wxWindow* const window = new (std::nothrow) GameFrame;
        SetTopWindow(window);
        return window;

    }

};

IMPLEMENT_APP(GameApp)
