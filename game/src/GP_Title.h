#ifndef GPTITLE_H_
#define GPTITLE_H_

#include "GamePanel.h"

class GP_Title : public GamePanel {

public:

    GP_Title(wxWindow* const parent);

protected:

    void DoPaint(wxDC& dc);
    void OnClick(wxMouseEvent& event);
    void ShowLevelLoadFailed();

};

#endif
