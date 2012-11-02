#include "GamePanel.h"

GamePanel::GamePanel(wxWindow* const parent) : wxPanel(parent, wxID_ANY, wxPoint(0, 0), parent->GetSize()) {

    Bind(wxEVT_PAINT, &GamePanel::OnPaint, this);
    Bind(wxEVT_ERASE_BACKGROUND, &GamePanel::OnBackgroundErase, this);

}

GamePanel::~GamePanel() {

}

void GamePanel::OnPaint(wxPaintEvent&) {

    wxAutoBufferedPaintDC dc(this);
    DoPaint(dc);

}