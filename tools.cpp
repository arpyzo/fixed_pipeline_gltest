#include "tools.h"


/***************************** RGB_Frame ******************************/

BEGIN_EVENT_TABLE(RGB_Frame, wxMiniFrame)
  EVT_SPIN_UP    (wxID_ANY, RGB_Frame::Event_Spin_Up)
  EVT_SPIN_DOWN  (wxID_ANY, RGB_Frame::Event_Spin_Down)
END_EVENT_TABLE()

RGB_Frame::RGB_Frame(wxWindow *parent)
: wxMiniFrame(parent, -1, "RGBA", wxDefaultPosition, wxSize(103,116))
{ wxPanel *main_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer *main_vsizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *hsizer;
  wxSpinButton *red_spinner, *green_spinner, *blue_spinner, *alpha_spinner;

  red_text_ctrl = new wxTextCtrl(main_panel, wxID_ANY, "0.40", wxDefaultPosition, wxSize(35,-1));
  red_spinner = new wxSpinButton(main_panel, R_SPIN, wxDefaultPosition, wxSize(-1,21), wxSP_VERTICAL | wxSP_ARROW_KEYS);
  
  hsizer = new wxBoxSizer(wxHORIZONTAL);
  hsizer->Add(new wxStaticText(main_panel, wxID_ANY, " Red:", wxDefaultPosition, wxSize(40,-1)), 0, wxALL | wxALIGN_CENTER, 0);
  hsizer->Add(red_text_ctrl, 0, 0, 0);
  hsizer->Add(red_spinner, 0, wxLEFT, 1);
  main_vsizer->Add(hsizer, 0, wxALL, 1);

  green_text_ctrl = new wxTextCtrl(main_panel, wxID_ANY, "0.40", wxDefaultPosition, wxSize(35,-1));
  green_spinner = new wxSpinButton(main_panel, G_SPIN, wxDefaultPosition, wxSize(-1,21), wxSP_VERTICAL | wxSP_ARROW_KEYS);
  
  hsizer = new wxBoxSizer(wxHORIZONTAL);
  hsizer->Add(new wxStaticText(main_panel, wxID_ANY, " Green:", wxDefaultPosition, wxSize(40,-1)), 0, wxALL | wxALIGN_CENTER, 0);
  hsizer->Add(green_text_ctrl, 0, 0, 0);
  hsizer->Add(green_spinner, 0, wxLEFT, 1);
  main_vsizer->Add(hsizer, 0, wxLEFT | wxRIGHT | wxBOTTOM, 1);

  blue_text_ctrl = new wxTextCtrl(main_panel, wxID_ANY, "0.40", wxDefaultPosition, wxSize(35,-1));
  blue_spinner = new wxSpinButton(main_panel, B_SPIN, wxDefaultPosition, wxSize(-1,21), wxSP_VERTICAL | wxSP_ARROW_KEYS);
  
  hsizer = new wxBoxSizer(wxHORIZONTAL);
  hsizer->Add(new wxStaticText(main_panel, wxID_ANY, " Blue:", wxDefaultPosition, wxSize(40,-1)), 0, wxALL | wxALIGN_CENTER, 0);
  hsizer->Add(blue_text_ctrl, 0, 0, 0);
  hsizer->Add(blue_spinner, 0, wxLEFT, 1);
  main_vsizer->Add(hsizer, 0, wxLEFT | wxRIGHT | wxBOTTOM, 1);

  alpha_text_ctrl = new wxTextCtrl(main_panel, wxID_ANY, "1.00", wxDefaultPosition, wxSize(35,-1));
  alpha_spinner = new wxSpinButton(main_panel, A_SPIN, wxDefaultPosition, wxSize(-1,21), wxSP_VERTICAL | wxSP_ARROW_KEYS);
  
  hsizer = new wxBoxSizer(wxHORIZONTAL);
  hsizer->Add(new wxStaticText(main_panel, wxID_ANY, " Alpha:", wxDefaultPosition, wxSize(40,-1)), 0, wxALL | wxALIGN_CENTER, 0);
  hsizer->Add(alpha_text_ctrl, 0, 0, 0);
  hsizer->Add(alpha_spinner, 0, wxLEFT, 1);
  main_vsizer->Add(hsizer, 0, wxLEFT | wxRIGHT | wxBOTTOM, 1);

  main_panel->SetSizer(main_vsizer);
}

/*****************/
/* Event_Spin_Up */
/*****************/
void RGB_Frame::Event_Spin_Up(wxSpinEvent &event)
{ wxTextCtrl *color_text_ctrl;
  double value;

  switch(event.GetId())
  { case R_SPIN: color_text_ctrl = red_text_ctrl;
      break;
    case G_SPIN: color_text_ctrl = green_text_ctrl;
      break;
    case B_SPIN: color_text_ctrl = blue_text_ctrl;
      break;
    case A_SPIN: color_text_ctrl = alpha_text_ctrl;
      break;
    default:
      return;
  }
    
  if (!color_text_ctrl->GetValue().ToDouble(&value))
  { if (event.GetId() != A_SPIN)
      value = 0.35;
    else
      value = 0.95;
  }
  value += 0.05;
  if (value > 1)
    value = 1;

  color_text_ctrl->SetValue(wxString::Format("%.2f", value));
}

/*******************/
/* Event_Spin_Down */
/*******************/
void RGB_Frame::Event_Spin_Down(wxSpinEvent &event)
{ wxTextCtrl *color_text_ctrl;
  double value;

  switch(event.GetId())
  { case R_SPIN: color_text_ctrl = red_text_ctrl;
      break;
    case G_SPIN: color_text_ctrl = green_text_ctrl;
      break;
    case B_SPIN: color_text_ctrl = blue_text_ctrl;
      break;
    case A_SPIN: color_text_ctrl = alpha_text_ctrl;
      break;
    default:
      return;
  }
    
  if (!color_text_ctrl->GetValue().ToDouble(&value))
  { if (event.GetId() != A_SPIN)
      value = 0.45;
    else
      value = 1.05;
  }
  value -= 0.05;
  if (value < 0)
    value = 0;

  color_text_ctrl->SetValue(wxString::Format("%.2f", value));
}

void RGB_Frame::Get_Values(float rgba[4])
{ double value[4];

  if (!red_text_ctrl->GetValue().ToDouble(&value[0]))
    value[0] = 0.40f;
  if (!green_text_ctrl->GetValue().ToDouble(&value[1]))
    value[1] = 0.40f;
  if (!blue_text_ctrl->GetValue().ToDouble(&value[2]))
    value[2] = 0.40f;
  if (!alpha_text_ctrl->GetValue().ToDouble(&value[3]))
    value[3] = 1.00f;

  for (int count = 0 ; count < 4 ; count++)
    rgba[count] = (float)value[count];

//wxMessageBox(wxString::Format("R - %.2f\nG - %.2f\nB - %.2f\nA - %.2f\n", rgba[0], rgba[1], rgba[2], rgba[3]));

}
