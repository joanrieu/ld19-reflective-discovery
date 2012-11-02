#include "GameFrame.h"
#include "GameConstants.h"
#include "GP_Title.h"

#include <wx/sound.h>
#include <wx/msgdlg.h>

GameFrame::GameFrame() : wxFrame(0, wxID_ANY, GC_TEXT_NAME, wxDefaultPosition, wxSize(GC_FRAME_WIDTH, GC_FRAME_HEIGHT), wxSYSTEM_MENU | wxCAPTION | wxMINIMIZE_BOX | wxCLOSE_BOX) {

    SetSize(ClientToWindowSize(GetSize()));

    Play();
    Bind(wxEVT_KEY_UP, &GameFrame::ToggleSound, this);

    Bind(wxEVT_CLOSE_WINDOW, &GameFrame::OnClose, this);

    Show();
    new GP_Title(this);

}

GameFrame::~GameFrame() {

}

void GameFrame::Play() {

    m_sound = wxSound::Play(GC_ART_MUSIC, wxSOUND_ASYNC | wxSOUND_LOOP);

}

void GameFrame::ToggleSound(wxKeyEvent& event) {

    if (wxString(event.GetUnicodeKey()).MakeLower() != wxString('s'))
        return;

    if (not m_sound)
        return Play();

    m_sound = not m_sound;
    wxSound::Stop();

}

void GameFrame::OnClose(wxCloseEvent& event) {

    if (event.CanVeto() and wxMessageBox(GC_TEXT_QUITCONFIRM, GC_TEXT_NAME, wxYES_NO | wxICON_QUESTION, this) != wxYES)
        return event.Veto();

    event.Skip();

}
