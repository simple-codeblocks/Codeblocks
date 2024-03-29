/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 8300 $
 * $Id: cbexception.cpp 8300 2012-08-31 11:35:40Z jenslody $
 * $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/sdk/cbexception.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "cbexception.h"
    #include "globals.h"
    #include "configmanager.h"
    #include <wx/log.h> // for wxSafeShowMessage()
#endif

#include <wx/intl.h>

cbException::cbException(const wxString& msg, const wxString& file, int line)
  : Message(msg),
    File(file),
    Line(line)
{}

cbException::~cbException()
{}


void cbException::ShowErrorMessage(bool safe)
{
    wxString gccvers;
#ifdef __GNUC__
    gccvers.Printf(_T("gcc %d.%d.%d"), __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif

    wxString title = _("Exception");
    wxString err;
    err.Printf(_("An exception has been raised!\n\n"
                 "The application encountered an error at %s, on line %d.\n"
                 "The error message is:\n\n%s\n\n"
                 "Code::Blocks Version revision %u (%s, "
                 "build: %s %s)"),
               File.c_str(), Line, Message.c_str(),
               ConfigManager::GetRevisionNumber(), gccvers.c_str(),
               wxT(__DATE__), wxT(__TIME__));
    if (safe)
        wxSafeShowMessage(title, err);
    else
        cbMessageBox(err, title, wxICON_ERROR);
}
