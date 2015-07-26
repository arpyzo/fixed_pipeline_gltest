#ifndef __TOOLS_H
#define __TOOLS_H

#include <wx/wx.h>
#include <wx/minifram.h>
#include <wx/spinbutt.h>

enum
{ R_SPIN,
  G_SPIN,
  B_SPIN,
  A_SPIN
};

/************************** RGB_Frame **********************************/
class RGB_Frame: public wxMiniFrame
{ public:
    RGB_Frame(wxWindow *parent);
    void Event_Spin_Up(wxSpinEvent &event);
    void Event_Spin_Down(wxSpinEvent &event);
    void Get_Values(float rgba[4]);

  private:
    wxTextCtrl *red_text_ctrl;
    wxTextCtrl *green_text_ctrl;
    wxTextCtrl *blue_text_ctrl;
    wxTextCtrl *alpha_text_ctrl;

  DECLARE_EVENT_TABLE()
};

#endif // __TOOLS_H
