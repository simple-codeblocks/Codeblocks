#ifndef WXDATETIMEPICKERCTRL_H
#define WXDATETIMEPICKERCTRL_H

#include <wx/datectrl.h>

class wxDateTimePickerCtrl : public wxDatePickerCtrlBase
{
    public:
        wxDateTimePickerCtrl( void);

        wxDateTimePickerCtrl(wxWindow *parent,
                         wxWindowID id,
                         wxString Format,
                         const wxDateTime& dt = wxDateTime::Now(),
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& size = wxDefaultSize,
                         long style = wxDP_DEFAULT | wxDP_SHOWCENTURY,
                         const wxValidator& validator = wxDefaultValidator,
                         const wxString& name = wxDatePickerCtrlNameStr);

        bool Create(wxWindow *parent,
                         wxWindowID id,
                         const wxDateTime& dt = wxDefaultDateTime,
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& size = wxDefaultSize,
                         long style = wxDP_DEFAULT | wxDP_SHOWCENTURY,
                         const wxValidator& validator = wxDefaultValidator,
                         const wxString& name = wxDatePickerCtrlNameStr);

        virtual void SetValue( const wxDateTime& dt);
        virtual wxDateTime GetValue( void) const;

        virtual void SetRange( const wxDateTime& dt1, const wxDateTime& dt2);
        virtual bool GetRange( wxDateTime* dt1, wxDateTime* dt2) const;

        virtual WXDWORD MSWGetStyle( long style, WXDWORD* exstyle) const;

        virtual bool MSWOnNotify( int idCtrl, WXLPARAM lParam, WXLPARAM* result);

    protected:

        virtual wxSize DoGetBestSize( void) const;

        wxDateTime m_date;
        wxString   m_Format;
};

#endif // WXTIMEPICKERCTRL_H
