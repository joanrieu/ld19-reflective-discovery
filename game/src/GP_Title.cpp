#include "GP_Title.h"
#include "GameConstants.h"
#include "GP_Game.h"

#include <wx/msgdlg.h>
#include <wx/textdlg.h>

GP_Title::GP_Title(wxWindow* const parent) : GamePanel(parent) {

    Bind(wxEVT_LEFT_UP, &GP_Title::OnClick, this);
    Bind(wxEVT_RIGHT_UP, &GP_Title::OnClick, this);

}

void GP_Title::DoPaint(wxDC& dc) {

    static wxBitmap logo(GC_ART_TITLE);
    dc.DrawBitmap(logo, wxPoint());

}

void GP_Title::OnClick(wxMouseEvent& event) {

    unsigned long level_id = 1;

    if (event.GetButton() == wxMOUSE_BTN_RIGHT) {

        wxTextEntryDialog dialog(this, GC_TEXT_LEVELCHOICE, GC_TEXT_NAME, wxString() << level_id);

        if (dialog.ShowModal() != wxID_OK)
            return;

        if (not dialog.GetValue().ToULong(&level_id))
            return ShowLevelLoadFailed();

    }

    GLevel* const level = GLevel::NewFromId(level_id);
    if (not level)
        return ShowLevelLoadFailed();

    new GP_Game(level, GetParent());
    Destroy();

}

void GP_Title::ShowLevelLoadFailed() {

    wxMessageBox(GC_TEXT_LEVELERROR, GC_TEXT_NAME, wxOK | wxICON_ERROR, this);

}
