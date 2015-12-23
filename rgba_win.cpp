#include "rgba_win.h"
#include "debug.h"

/***************************** RGBA_Win ******************************/
RGBA_Win::RGBA_Win(wxWindow *parent)
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

void RGBA_Win::Get_Values(float rgba[4]) {
    rgba[0] = red_spin_ctrl->GetValue();
    rgba[1] = green_spin_ctrl->GetValue();
    rgba[2] = blue_spin_ctrl->GetValue();
    rgba[3] = alpha_spin_ctrl->GetValue();
}
