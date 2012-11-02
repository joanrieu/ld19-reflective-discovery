#ifndef GAMEFRAME_H_
#define GAMEFRAME_H_

#include <wx/frame.h>

class GameFrame : public wxFrame {

public:

    GameFrame();
    ~GameFrame();

private:

    bool m_sound;
    void Play();
    void ToggleSound(wxKeyEvent& event);

    void OnClose(wxCloseEvent& event);

};

#endif
