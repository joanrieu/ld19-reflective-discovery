#include "GP_Game.h"
#include "GameConstants.h"
#include "GP_Title.h"

#include <wx/msgdlg.h>

GP_Game::GP_Game(GLevel* const level, wxWindow* const parent) : GamePanel(parent), m_game(new GGame(level)) {

    m_background = new wxBitmap(GC_ART_BACKGROUND);
    m_laser = new wxImage(GC_ART_LASER);

    LoadImages(m_lasers, GC_ART_LASERS);
    LoadImages(m_lasers_light, GC_ART_LASERS_LIGHT);
    LoadImages(m_reflectors, GC_ART_REFLECTORS);
    LoadImages(m_blocks, GC_ART_BLOCKS);
    LoadImages(m_blocks_light, GC_ART_BLOCKS_LIGHT);

    Bind(wxEVT_LEFT_UP, &GP_Game::OnLeftClick, this);
    Bind(wxEVT_RIGHT_UP, &GP_Game::OnRightClick, this);

}

void GP_Game::LoadImages(ColorImgCont& cont, const wxString& name) {

    for (GCol col = GC_COL_RED; col < GC_COL_WHITE; col = static_cast<GCol>(col+1))
        cont.insert(std::make_pair(col, new wxImage(GC_ART_AUTO_PNG(name << col))));

}

GP_Game::~GP_Game() {

    delete m_game;

    for (ColorImgCont::const_iterator it = m_blocks_light.begin(); it != m_blocks_light.end(); ++it)
        delete it->second;

    for (ColorImgCont::const_iterator it = m_blocks.begin(); it != m_blocks.end(); ++it)
        delete it->second;

    for (ColorImgCont::const_iterator it = m_reflectors.begin(); it != m_reflectors.end(); ++it)
        delete it->second;

    for (ColorImgCont::const_iterator it = m_lasers_light.begin(); it != m_lasers_light.end(); ++it)
        delete it->second;

    for (ColorImgCont::const_iterator it = m_lasers.begin(); it != m_lasers.end(); ++it)
        delete it->second;

    delete m_background;
    delete m_laser;

}

void GP_Game::DoPaint(wxDC& dc) {

    dc.DrawBitmap(*m_background, 0, 0);

    dc.DrawBitmap(
        ImageToRotatedBmp(*m_laser, m_game->level->laser.second.dir),
        wxPoint(m_game->level->laser.first.first, m_game->level->laser.first.second) * GC_LEVEL_BLOCK_SIZE
    );

    for (GGame::GRayCont::const_iterator it = m_game->rays.begin(); it != m_game->rays.end(); ++it)
        dc.DrawBitmap(
            ImageToRotatedBmp(*(it->second.lit_up ? m_lasers_light : m_lasers)[it->second.col], it->second.dir),
            wxPoint(it->first.first, it->first.second) * GC_LEVEL_BLOCK_SIZE
        );

    for (GLevel::GReflectorCont::const_iterator it = m_game->level->reflectors.begin(); it != m_game->level->reflectors.end(); ++it) {
        dc.DrawBitmap(
            ImageToRotatedBmp(*m_reflectors[it->second.col], it->second.dir),
            wxPoint(it->first.first, it->first.second) * GC_LEVEL_BLOCK_SIZE
        );
        dc.DrawBitmap(
            ImageToRotatedBmp(*m_reflectors[it->second.col_alt], it->second.dir, true),
            wxPoint(it->first.first, it->first.second) * GC_LEVEL_BLOCK_SIZE
        );
    }

    for (GLevel::GBlockCont::const_iterator it = m_game->blocks.begin(); it != m_game->blocks.end(); ++it)
        dc.DrawBitmap(
            *(it->second.lit_up ? m_blocks_light : m_blocks)[it->second.col],
            wxPoint(it->first.first, it->first.second) * GC_LEVEL_BLOCK_SIZE
        );

}

wxBitmap GP_Game::ImageToRotatedBmp(const wxImage& source, const GDir& dir, bool halfturn) {

    wxImage image(source);

    if (dir == GC_DIR_LEFT)
        image = image.Rotate90(false);

    if (dir == GC_DIR_RIGHT or dir == GC_DIR_DOWN)
        image = image.Rotate90();

    if (dir == GC_DIR_DOWN)
        image = image.Rotate90();

    if (halfturn)
        image = image.Rotate90().Rotate90();

    return wxBitmap(image);

}

void GP_Game::OnLeftClick(wxMouseEvent& event) {

    GLevel::GReflectorCont::iterator it = m_game->level->reflectors.find(
        std::make_pair(static_cast<GCoord>(event.GetX() / GC_LEVEL_BLOCK_SIZE), static_cast<GCoord>(event.GetY() / GC_LEVEL_BLOCK_SIZE))
    );

    if (it != m_game->level->reflectors.end()) {
        it->second.dir = static_cast<GDir>((it->second.dir + 1) % 4);
        Refresh();
    }

}

void GP_Game::Refresh() {

    m_game->Refresh();
    wxPanel::Refresh();

    if (not m_game->won)
        return;

    GLevel* const level = GLevel::NewFromId(m_game->level->id + 1);

    if (not level) {
        wxMessageBox(GC_TEXT_CONGRATULATIONS(m_game->level->id), GC_TEXT_NAME, wxOK | wxICON_INFORMATION, this);
        new GP_Title(GetParent());
    } else {
        wxMessageBox(GC_TEXT_CONGRATULATIONS(m_game->level->id) << "\n" << GC_TEXT_NEXTLEVEL(m_game->level->id + 1), GC_TEXT_NAME, wxOK | wxICON_INFORMATION, this);
        new GP_Game(level, GetParent());
    }

    Destroy();

}

void GP_Game::OnRightClick(wxMouseEvent&) {

    if (wxMessageBox(GC_TEXT_LEVELQUITCONFIRM, GC_TEXT_NAME, wxYES_NO | wxICON_QUESTION, this) != wxYES)
        return;

    new GP_Title(GetParent());
    Destroy();

}
