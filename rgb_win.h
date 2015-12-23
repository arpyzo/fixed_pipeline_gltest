#ifndef __RGB_WIN_H
#define __RGB_WIN_H

#include <wx/wx.h>
#include <wx/minifram.h>
//#include <wx/spinbutt.h>
#include <wx/spinctrl.h>

/************************** RGB_Frame **********************************/
class RGB_Win: public wxMiniFrame {
    public:
        enum {
            RED_SPINNER,
            GREEN_SPINNER,
            BLUE_SPINNER,
            ALPHA_SPINNER
        };

        RGB_Win(wxWindow *parent);

        //void Event_Spin(wxSpinEvent &event);
        //void Event_Spin_Up(wxSpinEvent &event);
        //void Event_Spin_Down(wxSpinEvent &event);
        void Get_Values(float rgba[4]);

    private:
        //wxTextCtrl *red_text_ctrl;
        //wxTextCtrl *green_text_ctrl;
        //wxTextCtrl *blue_text_ctrl;
        //wxTextCtrl *alpha_text_ctrl;

        wxSpinCtrlDouble *red_spin_ctrl;
        wxSpinCtrlDouble *green_spin_ctrl;
        wxSpinCtrlDouble *blue_spin_ctrl;
        wxSpinCtrlDouble *alpha_spin_ctrl;

    //DECLARE_EVENT_TABLE()
};

#endif // __RGB_WIN_H
