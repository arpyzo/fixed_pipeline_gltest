#include "rgb_win.h"

/***************************** RGB_Win ******************************/
//BEGIN_EVENT_TABLE(RGB_Win, wxMiniFrame)
//    EVT_SPIN_UP(wxID_ANY, RGB_Win::Event_Spin_Up)
//    EVT_SPIN_DOWN(wxID_ANY, RGB_Win::Event_Spin_Down)
//END_EVENT_TABLE()

RGB_Win::RGB_Win(wxWindow *parent)
: wxMiniFrame(parent, -1, "RGBA", wxDefaultPosition, wxSize(115,138), wxCAPTION | wxSTAY_ON_TOP) {
    wxPanel *main_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *main_vsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hsizer;

    red_spin_ctrl = new wxSpinCtrlDouble(main_panel, wxID_ANY, "0.40", wxDefaultPosition, wxSize(53, 21), wxSP_ARROW_KEYS, 0, 1, 0.4, 0.05);
    red_spin_ctrl->SetDigits(2);
  
    hsizer = new wxBoxSizer(wxHORIZONTAL);
    hsizer->Add(new wxStaticText(main_panel, wxID_ANY, " Red:", wxDefaultPosition, wxSize(40,-1)), 0, wxALL | wxALIGN_CENTER, 0);
    hsizer->Add(red_spin_ctrl, 0, wxLEFT, 1);
    main_vsizer->Add(hsizer, 0, wxTOP, 6);

    green_spin_ctrl = new wxSpinCtrlDouble(main_panel, wxID_ANY, "0.40", wxDefaultPosition, wxSize(53, 21), wxSP_ARROW_KEYS, 0, 1, 0.4, 0.05);
    green_spin_ctrl->SetDigits(2);

    hsizer = new wxBoxSizer(wxHORIZONTAL);
    hsizer->Add(new wxStaticText(main_panel, wxID_ANY, " Green:", wxDefaultPosition, wxSize(40,-1)), 0, wxALL | wxALIGN_CENTER, 0);
    hsizer->Add(green_spin_ctrl, 0, wxLEFT, 1);
    main_vsizer->Add(hsizer, 0, wxTOP | wxLEFT | wxRIGHT, 1);

    blue_spin_ctrl = new wxSpinCtrlDouble(main_panel, wxID_ANY, "0.40", wxDefaultPosition, wxSize(53, 21), wxSP_ARROW_KEYS, 0, 1, 0.4, 0.05);
    blue_spin_ctrl->SetDigits(2);

    hsizer = new wxBoxSizer(wxHORIZONTAL);
    hsizer->Add(new wxStaticText(main_panel, wxID_ANY, " Blue:", wxDefaultPosition, wxSize(40,-1)), 0, wxALL | wxALIGN_CENTER, 0);
    hsizer->Add(blue_spin_ctrl, 0, wxLEFT, 1);
    main_vsizer->Add(hsizer, 0, wxTOP | wxLEFT | wxRIGHT, 1);

    alpha_spin_ctrl = new wxSpinCtrlDouble(main_panel, wxID_ANY, "1.00", wxDefaultPosition, wxSize(53, 21), wxSP_ARROW_KEYS, 0, 1, 1, 0.05);
    alpha_spin_ctrl->SetDigits(2);

    hsizer = new wxBoxSizer(wxHORIZONTAL);
    hsizer->Add(new wxStaticText(main_panel, wxID_ANY, " Alpha:", wxDefaultPosition, wxSize(40,-1)), 0, wxALL | wxALIGN_CENTER, 0);
    hsizer->Add(alpha_spin_ctrl, 0, wxLEFT, 1);
    main_vsizer->Add(hsizer, 0, wxTOP | wxLEFT | wxRIGHT, 1);

    main_panel->SetSizer(main_vsizer);
}


/*****************/
/* Event_Spin_Up */
/*****************/
/*void RGB_Win::Event_Spin_Up(wxSpinEvent &event) {
    wxTextCtrl *color_text_ctrl;
    double value;

    switch(event.GetId()) {
        case RED_SPINNER: color_text_ctrl = red_text_ctrl;
            break;
        case GREEN_SPINNER: color_text_ctrl = green_text_ctrl;
            break;
        case BLUE_SPINNER: color_text_ctrl = blue_text_ctrl;
            break;
        case ALPHA_SPINNER: color_text_ctrl = alpha_text_ctrl;
            break;
        default:
            return;
    }
    
    if (!color_text_ctrl->GetValue().ToDouble(&value)) {
        if (event.GetId() == ALPHA_SPINNER) {
            value = 0.95;
        } else {
            value = 0.35;
        }
    }

    value += 0.05;
    if (value > 1) {
        value = 1;
    }

    color_text_ctrl->SetValue(wxString::Format("%.2f", value));
}*/

/*******************/
/* Event_Spin_Down */
/*******************/
/*void RGB_Win::Event_Spin_Down(wxSpinEvent &event)
{ wxTextCtrl *color_text_ctrl;
  double value;

  switch(event.GetId())
  { case RED_SPINNER: color_text_ctrl = red_text_ctrl;
      break;
    case GREEN_SPINNER: color_text_ctrl = green_text_ctrl;
      break;
    case BLUE_SPINNER: color_text_ctrl = blue_text_ctrl;
      break;
    case ALPHA_SPINNER: color_text_ctrl = alpha_text_ctrl;
      break;
    default:
      return;
  }
    
  if (!color_text_ctrl->GetValue().ToDouble(&value))
  { if (event.GetId() != ALPHA_SPINNER)
      value = 0.45;
    else
      value = 1.05;
  }
  value -= 0.05;
  if (value < 0)
    value = 0;

  color_text_ctrl->SetValue(wxString::Format("%.2f", value));
}*/

void RGB_Win::Get_Values(float rgba[4])
{ /*double value[4];

  if (!red_text_ctrl->GetValue().ToDouble(&value[0]))
    value[0] = 0.40f;
  if (!green_text_ctrl->GetValue().ToDouble(&value[1]))
    value[1] = 0.40f;
  if (!blue_text_ctrl->GetValue().ToDouble(&value[2]))
    value[2] = 0.40f;
  if (!alpha_text_ctrl->GetValue().ToDouble(&value[3]))
    value[3] = 1.00f;

  for (int count = 0 ; count < 4 ; count++)
    rgba[count] = (float)value[count];*/

  rgba[0] = red_spin_ctrl->GetValue();
  rgba[1] = green_spin_ctrl->GetValue();
  rgba[2] = blue_spin_ctrl->GetValue();
  rgba[3] = alpha_spin_ctrl->GetValue();

//wxMessageBox(wxString::Format("R - %.2f\nG - %.2f\nB - %.2f\nA - %.2f\n", rgba[0], rgba[1], rgba[2], rgba[3]));

}
