#ifndef GAMEPANEL_H_
#define GAMEPANEL_H_

#include <wx/panel.h>
#include <wx/dcbuffer.h>

class GamePanel : public wxPanel {

public:

    GamePanel(wxWindow* const parent);
    virtual ~GamePanel();

protected:

    virtual void DoPaint(wxDC& dc) = 0;

private:

    void OnPaint(wxPaintEvent& event);
    void OnBackgroundErase(wxEraseEvent&) {}

};

#endif
