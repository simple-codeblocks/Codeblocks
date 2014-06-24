/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 7443 $
 * $Id: scriptsecuritywarningdlg.cpp 7443 2011-09-01 16:29:16Z mortenmacfly $
 * $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/sdk/scriptsecuritywarningdlg.cpp $
 */

#include "sdk_precomp.h"
#include "scriptsecuritywarningdlg.h"

#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>

ScriptSecurityWarningDlg::ScriptSecurityWarningDlg(wxWindow* parent, const wxString& operation, const wxString& command)
{
    //ctor
    wxXmlResource::Get()->LoadObject(this, parent, _T("ScriptingSecurityDlg"),_T("wxScrollingDialog"));

    wxColour c = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
    XRCCTRL(*this, "txtCommand", wxTextCtrl)->SetBackgroundColour(c);

    XRCCTRL(*this, "lblOperation", wxStaticText)->SetLabel(_("Operation: ") + operation);
    XRCCTRL(*this, "txtCommand", wxTextCtrl)->SetValue(command);
}

ScriptSecurityWarningDlg::~ScriptSecurityWarningDlg()
{
    //dtor
}

ScriptSecurityResponse ScriptSecurityWarningDlg::GetResponse()
{
    return (ScriptSecurityResponse)XRCCTRL(*this, "cmbAnswer", wxComboBox)->GetSelection();
}

void ScriptSecurityWarningDlg::EndModal(int retCode)
{
    wxScrollingDialog::EndModal(retCode);
}
