#ifndef __RGBA_WIN_H
#define __RGBA_WIN_H

#include <wx/wx.h>
#include <wx/minifram.h>
#include <wx/spinctrl.h>

/************************** RGBA_Win **********************************/
class RGBA_Win: public wxMiniFrame {
    public:
        RGBA_Win(wxWindow *parent);

        void Get_Values(float rgba[4]);

    private:
        wxSpinCtrlDouble *red_spin_ctrl;
        wxSpinCtrlDouble *green_spin_ctrl;
        wxSpinCtrlDouble *blue_spin_ctrl;
        wxSpinCtrlDouble *alpha_spin_ctrl;
};

#endif // __RGBA_WIN_H
