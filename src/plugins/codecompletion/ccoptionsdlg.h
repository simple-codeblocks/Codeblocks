/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CCOPTIONSDLG_H
#define CCOPTIONSDLG_H

#include <wx/intl.h>
#include "configurationpanel.h"
#include <settings.h>
#include "nativeparser.h"
#include "parser/parser.h"

class CodeCompletion;
class DocumentationHelper;

class CCOptionsDlg : public cbConfigurationPanel
{
public:
    CCOptionsDlg(wxWindow* parent, NativeParser* np, CodeCompletion* cc, DocumentationHelper* dh);
    virtual ~CCOptionsDlg();

    virtual wxString GetTitle() const          { return _("Code completion"); }
    virtual wxString GetBitmapBaseName() const { return _T("codecompletion"); }
    virtual void OnApply();
    virtual void OnCancel()                    { ; }

protected:
    void OnAddRepl(wxCommandEvent& event);
    void OnEditRepl(wxCommandEvent& event);
    void OnDelRepl(wxCommandEvent& event);
    void OnChooseColour(wxCommandEvent& event);
    void OnCCDelayScroll(wxScrollEvent& event);

    void OnUpdateUI(wxUpdateUIEvent& event);

private:
    void UpdateCCDelayLabel();
    bool ValidateReplacementToken(wxString& from, wxString& to);

    NativeParser*        m_NativeParser;
    CodeCompletion*      m_CodeCompletion;
    ParserBase&          m_Parser;
    DocumentationHelper* m_Documentation;

    DECLARE_EVENT_TABLE()
};

#endif // CCOPTIONSDLG_H
