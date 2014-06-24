/////////////////////////////////////////////////////////////////////////////
// Name:        wxMenuButton
// Purpose:     A button with a dropdown wxMenu
// Author:      John Labenski
// Modified by:
// Created:     11/05/2002
// Copyright:   (c) John Labenski
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

/*

wxMenuButton is a button that drops down an assigned wxMenu

Create the button with either a text or bitmap label.
    Create a new wxMenu and call AssignMenu and thats it. When you press the
    dropdown button the menu appears. When you press the label button the next
    wxITEM_RADIO (ie wxMenuItem::GetKind) in the menu is selected round robin.
    If there are no radio items then it really just acts like a menubar, though
    this is probably not too useful. The events sent in this case are EVT_MENUs
    either generated by the menu when you click on it or created when you click
    on the label to select the next radio item.
*/

#ifndef _WX_MENUBTN_H_
#define _WX_MENUBTN_H_

#include "wx/things/thingdef.h"
#include "wx/things/toggle.h"

class WXDLLIMPEXP_FWD_CORE wxMenu;
class WXDLLIMPEXP_FWD_CORE wxBitmap;

//-----------------------------------------------------------------------------
// wxMenuButton styles
//-----------------------------------------------------------------------------

#define wxMENUBUTTON_DROP_WIDTH  10
#define wxMENUBUTTON_DROP_HEIGHT 22

enum wxMenuButton_Styles
{
    wxMENUBUT_FLAT = wxCUSTBUT_FLAT
};

//-----------------------------------------------------------------------------
// wxMenuButton
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_THINGS wxMenuButton : public wxControl
{
public:

    wxMenuButton() : wxControl() { Init(); }

    // Use this constructor if you need one compatible with a wxButton
    //   setup the button later with AssignMenu
    wxMenuButton( wxWindow* parent,
                  wxWindowID id,
                  const wxString &label,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxValidator& val = wxDefaultValidator,
                  const wxString& name = wxT("wxMenuButton"))
                  : wxControl()
    {
        Init();
        Create(parent,id,label,wxNullBitmap,pos,size,style,val,name);
    }

    // Use this constructor if you need one compatible with a wxBitmapButton
    //   setup the button later with AssignMenu
    wxMenuButton( wxWindow* parent, wxWindowID id,
                  const wxBitmap &bitmap,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxValidator& val = wxDefaultValidator,
                  const wxString& name = wxT("wxMenuButton"))
                  : wxControl()
    {
        Init();
        Create(parent,id,wxEmptyString,bitmap,pos,size,style,val,name);
    }

    // wxComboBox constructor, completely setup later
    wxMenuButton( wxWindow *parent, wxWindowID id,
                  const wxString& value = wxEmptyString,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  int n = 0, const wxString choices[] = (const wxString *) NULL,
                  long style = 0,
                  const wxValidator& val = wxDefaultValidator,
                  const wxString& name = wxT("wxMenuButton"))
                  : wxControl()
    {
        n = n; choices = choices; // get rid of unused warning
        Init();
        Create(parent, id, value, wxNullBitmap, pos, size, style, val, name);
    }

    // Native constructor
    wxMenuButton( wxWindow* parent, wxWindowID id,
                  const wxString &label, const wxBitmap &bitmap,
                  wxMenu *menu, bool static_menu = false,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxValidator& val = wxDefaultValidator,
                  const wxString& name = wxT("wxMenuButton"))
                  : wxControl()
    {
        Init();
        AssignMenu(menu, static_menu);
        Create(parent,id,label,bitmap,pos,size,style,val,name);
    }

    virtual ~wxMenuButton();

    bool Create( wxWindow* parent,
                 wxWindowID id,
                 const wxString &label,
                 const wxBitmap &bitmap,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxNO_BORDER,
                 const wxValidator& val = wxDefaultValidator,
                 const wxString& name = wxT("wxMenuButton"));

    // Gets the id of the first selected radio item or wxNOT_FOUND (-1) if none
    int GetSelection() const;

    // This menu will be displayed when the dropdown button is pressed.
    //   if static_menu is false it will be deleted when the buttton is destroyed.
    void AssignMenu(wxMenu *menu, bool static_menu = false);

    wxMenu *GetMenu() const { return m_menu; }

    // get a pointer to the label button, for turning it into a toggle perhaps
    wxCustomButton *GetLabelButton() const { return m_labelButton; }
    wxCustomButton *GetDropDownButton() const { return m_dropdownButton; }

    void SetToolTip(const wxString &tip);
    void SetToolTip(wxToolTip *tip);

protected:
    void OnButton(wxCommandEvent &event);

    virtual void DoSetSize(int x, int y, int width, int height,
                           int sizeFlags = wxSIZE_AUTO);

    virtual wxSize DoGetBestSize();

// FIXME! - in MSW the radio items don't check themselves
#ifdef __WXMSW__
    void OnMenu( wxCommandEvent &event );
#endif


    wxCustomButton *m_labelButton;
    wxCustomButton *m_dropdownButton;

    wxMenu *m_menu;
    bool m_menu_static;
    long m_style;

private:
    void Init();
    DECLARE_DYNAMIC_CLASS(wxMenuButton)
    DECLARE_EVENT_TABLE()
};

//-----------------------------------------------------------------------------
// wxMenuButtonEvents
//
// EVT_MENUBUTTON_OPEN(id, fn) - menu is about to be opened, (dis)(en)able items
//                               or call Veto() to stop menu from popping up
//                               this is a wxNotifyEvent
//-----------------------------------------------------------------------------

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_THINGS, wxEVT_MENUBUTTON_OPEN, 0)
END_DECLARE_EVENT_TYPES()

#define EVT_MENUBUTTON_OPEN(id, fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_MENUBUTTON_OPEN, id, wxID_ANY, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNotifyEventFunction) & fn, (wxObject *) NULL ),

#endif  // _WX_MENUBTN_H_
