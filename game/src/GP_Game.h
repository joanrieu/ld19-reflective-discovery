#ifndef GPGAME_H_
#define GPGAME_H_

#include "GamePanel.h"
#include "GGame.h"

#include <map>

class GP_Game : public GamePanel {

public:

    GP_Game(GLevel* const level, wxWindow* const parent);
    ~GP_Game();

protected:

    void DoPaint(wxDC& dc);

private:

    wxBitmap ImageToRotatedBmp(const wxImage& source, const GDir& dir, bool halfturn = false);

    void OnLeftClick(wxMouseEvent& event);
    void Refresh();

    void OnRightClick(wxMouseEvent& event);

    GGame* const m_game;

    wxImage* m_laser;
    wxBitmap* m_background;

    typedef std::map<GCol, wxImage*> ColorImgCont;
    void LoadImages(ColorImgCont& cont, const wxString& name);
    ColorImgCont m_reflectors;
    ColorImgCont m_lasers, m_lasers_light;
    ColorImgCont m_blocks, m_blocks_light;

};

#endif
