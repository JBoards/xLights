#include "HousePreviewPanel.h"
#include "xLightsMain.h"
#include "ModelPreview.h"
#include <wx/artprov.h>
#include "xLightsXmlFile.h"

//(*InternalHeaders(HousePreviewPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(HousePreviewPanel)
const long HousePreviewPanel::ID_BITMAPBUTTON1 = wxNewId();
const long HousePreviewPanel::ID_BITMAPBUTTON2 = wxNewId();
const long HousePreviewPanel::ID_BITMAPBUTTON3 = wxNewId();
const long HousePreviewPanel::ID_BITMAPBUTTON4 = wxNewId();
const long HousePreviewPanel::ID_BITMAPBUTTON6 = wxNewId();
const long HousePreviewPanel::ID_BITMAPBUTTON7 = wxNewId();
const long HousePreviewPanel::ID_SLIDER1 = wxNewId();
const long HousePreviewPanel::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(HousePreviewPanel,wxPanel)
	//(*EventTable(HousePreviewPanel)
	//*)
END_EVENT_TABLE()

HousePreviewPanel::HousePreviewPanel(wxWindow* parent, xLightsFrame* frame,
    bool showToolbars,
    std::vector<Model*> &models,
    std::vector<LayoutGroup *> &groups,
    bool allowSelected,
    int style,
    bool allowPreviewChange,
    wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    _xLights = frame;
    _showToolbar = showToolbars;

	//(*Initialize(HousePreviewPanel)
	wxFlexGridSizer* ModelPreviewSizer;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	ModelPreviewSizer = new wxFlexGridSizer(0, 1, 0, 0);
	ModelPreviewSizer->AddGrowableCol(0);
	ModelPreviewSizer->AddGrowableRow(0);
	FlexGridSizer1->Add(ModelPreviewSizer, 1, wxALL|wxEXPAND, 0);
	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	ButtonSizer = new wxFlexGridSizer(0, 2, 0, 0);
	ButtonSizer->AddGrowableCol(1);
	FlexGridSizer2 = new wxFlexGridSizer(0, 6, 0, 0);
	PlayButton = new wxBitmapButton(Panel1, ID_BITMAPBUTTON1, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
	PlayButton->SetMinSize(wxSize(24,24));
	FlexGridSizer2->Add(PlayButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	PauseButton = new wxBitmapButton(Panel1, ID_BITMAPBUTTON2, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
	PauseButton->SetMinSize(wxSize(24,24));
	FlexGridSizer2->Add(PauseButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StopButton = new wxBitmapButton(Panel1, ID_BITMAPBUTTON3, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON3"));
	StopButton->SetMinSize(wxSize(24,24));
	FlexGridSizer2->Add(StopButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	RewindButton = new wxBitmapButton(Panel1, ID_BITMAPBUTTON4, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON4"));
	RewindButton->SetMinSize(wxSize(24,24));
	FlexGridSizer2->Add(RewindButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Rewind10Button = new wxBitmapButton(Panel1, ID_BITMAPBUTTON6, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON6"));
	Rewind10Button->SetMinSize(wxSize(24,24));
	FlexGridSizer2->Add(Rewind10Button, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FastForward10Button = new wxBitmapButton(Panel1, ID_BITMAPBUTTON7, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON7"));
	FastForward10Button->SetMinSize(wxSize(24,24));
	FlexGridSizer2->Add(FastForward10Button, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSizer->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	SliderPosition = new wxSlider(Panel1, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER1"));
	ButtonSizer->Add(SliderPosition, 1, wxALL|wxEXPAND, 2);
	Panel1->SetSizer(ButtonSizer);
	ButtonSizer->Fit(Panel1);
	ButtonSizer->SetSizeHints(Panel1);
	FlexGridSizer1->Add(Panel1, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&HousePreviewPanel::OnPlayButtonClick);
	Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&HousePreviewPanel::OnPauseButtonClick);
	Connect(ID_BITMAPBUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&HousePreviewPanel::OnStopButtonClick);
	Connect(ID_BITMAPBUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&HousePreviewPanel::OnRewindButtonClick);
	Connect(ID_BITMAPBUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&HousePreviewPanel::OnRewind10ButtonClick);
	Connect(ID_BITMAPBUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&HousePreviewPanel::OnFastForward10ButtonClick);
	Connect(ID_SLIDER1,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&HousePreviewPanel::OnSliderPositionCmdSliderUpdated);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&HousePreviewPanel::OnResize);
	//*)

    _modelPreview = new ModelPreview(this, _xLights, models ,groups, allowSelected, style, allowPreviewChange);
    ModelPreviewSizer->Add(_modelPreview, 1, wxALL | wxEXPAND, 2);

    ValidateWindow(GetSize());

    PlayButton->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_PLAY"))));
    PauseButton->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_PAUSE"))));
    StopButton->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_STOP"))));
    RewindButton->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_BACKWARD"))));
    Rewind10Button->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_REWIND10"))));
    FastForward10Button->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_FFORWARD10"))));

    Rewind10Button->SetToolTip("Rewind 10 seconds.");
    FastForward10Button->SetToolTip("Fast forward 10 seconds.");

    Fit();
    Layout();
}

HousePreviewPanel::~HousePreviewPanel()
{
	//(*Destroy(HousePreviewPanel)
	//*)
}


void HousePreviewPanel::OnPlayButtonClick(wxCommandEvent& event)
{
    wxCommandEvent playEvent(EVT_PLAY_SEQUENCE);
    wxPostEvent(_xLights, playEvent);
}

void HousePreviewPanel::OnPauseButtonClick(wxCommandEvent& event)
{
    wxCommandEvent playEvent(EVT_PAUSE_SEQUENCE);
    wxPostEvent(_xLights, playEvent);
}

void HousePreviewPanel::OnStopButtonClick(wxCommandEvent& event)
{
    wxCommandEvent playEvent(EVT_STOP_SEQUENCE);
    wxPostEvent(_xLights, playEvent);
}

void HousePreviewPanel::OnRewindButtonClick(wxCommandEvent& event)
{
    SliderPosition->SetValue(0);
    _xLights->GetMainSequencer()->PanelTimeLine->ResetMarkers(0);
    wxCommandEvent playEvent(EVT_SEQUENCE_FIRST_FRAME);
    wxPostEvent(_xLights, playEvent);
}

void HousePreviewPanel::OnRewind10ButtonClick(wxCommandEvent& event)
{
    wxCommandEvent playEvent(EVT_SEQUENCE_REWIND10);
    wxPostEvent(_xLights, playEvent);
}

void HousePreviewPanel::OnFastForward10ButtonClick(wxCommandEvent& event)
{
    wxCommandEvent playEvent(EVT_SEQUENCE_FFORWARD10);
    wxPostEvent(_xLights, playEvent);
}

void HousePreviewPanel::OnEndButtonClick(wxCommandEvent& event)
{
    wxCommandEvent playEvent(EVT_SEQUENCE_LAST_FRAME);
    wxPostEvent(_xLights, playEvent);
}

void HousePreviewPanel::OnResize(wxSizeEvent& event)
{
    ValidateWindow(event.GetSize());
}

void HousePreviewPanel::ValidateWindow(const wxSize& size)
{
    if (_showToolbar)
    {
        if (size.GetWidth() > 400 && size.GetHeight() > 300 && ! _xLights->IsPaneDocked(this))
        {
            if (!Panel1->IsShown())
            {
                Panel1->Show();
            }
        }
        else
        {
            if (Panel1->IsShown())
            {
                Panel1->Hide();
            }
        }
    }
    else
    {
        if (Panel1->IsShown())
        {
            Panel1->Hide();
        }
    }
    Layout();
    ButtonSizer->Layout();
}

void HousePreviewPanel::EnablePlayControls(const std::string& control, bool enable)
{
    if (control == "Play")
    {
        PlayButton->Enable(enable);
    }
    else if (control == "Pause")
    {
        PauseButton->Enable(enable);
    }
    else if (control == "Stop")
    {
        StopButton->Enable(enable);
    }
    else if (control == "Rewind")
    {
        RewindButton->Enable(enable);
    }
    else if (control == "Rewind10")
    {
        Rewind10Button->Enable(enable);
    }
    else if (control == "FForward10")
    {
        FastForward10Button->Enable(enable);
    }
    else if (control == "Position")
    {
        SliderPosition->Enable(enable);
    }
}

void HousePreviewPanel::SetDurationFrames(long frames)
{
    SliderPosition->SetMax(frames);
}

void HousePreviewPanel::SetPositionFrames(long frames)
{
    SliderPosition->SetValue(frames);
}

void HousePreviewPanel::OnSliderPositionCmdSliderUpdated(wxScrollEvent& event)
{
    _xLights->GetMainSequencer()->PanelTimeLine->ResetMarkers(event.GetPosition() * 1000 / _xLights->CurrentSeqXmlFile->GetFrequency());
}