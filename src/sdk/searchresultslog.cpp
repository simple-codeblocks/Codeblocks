/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 9203 $
 * $Id: searchresultslog.cpp 9203 2013-07-08 23:07:22Z fuscated $
 * $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/sdk/searchresultslog.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/arrstr.h>
    #include <wx/filename.h>
    #include <wx/listctrl.h>
    #include "manager.h"
    #include "editormanager.h"
    #include "cbeditor.h"
#endif
#include "cbstyledtextctrl.h"

#include "searchresultslog.h"

namespace
{
    const int ID_List = wxNewId();
}

BEGIN_EVENT_TABLE(cbSearchResultsLog, wxEvtHandler)
//
END_EVENT_TABLE()

cbSearchResultsLog::cbSearchResultsLog(const wxArrayString& titles_in, wxArrayInt& widths_in)
    : ListCtrlLogger(titles_in, widths_in)
{
    //ctor
}

cbSearchResultsLog::~cbSearchResultsLog()
{
    //dtor
}

wxWindow* cbSearchResultsLog::CreateControl(wxWindow* parent)
{
    ListCtrlLogger::CreateControl(parent);
    control->SetId(ID_List);
    Connect(ID_List, -1, wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
            &cbSearchResultsLog::OnDoubleClick);
    Manager::Get()->GetAppWindow()->PushEventHandler(this);
    return control;
}

void cbSearchResultsLog::FocusEntry(size_t index)
{
    if (index < (size_t)control->GetItemCount())
    {
        control->SetItemState(index, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);
        control->EnsureVisible(index);
        SyncEditor(index);
    }
}

void cbSearchResultsLog::SyncEditor(int selIndex)
{
    wxFileName filename(control->GetItemText(selIndex));
    wxString file;
    if (!filename.IsAbsolute())
        filename.MakeAbsolute(m_Base);
    file = filename.GetFullPath();

    wxListItem li;
    li.m_itemId = selIndex;
    li.m_col = 1;
    li.m_mask = wxLIST_MASK_TEXT;
    control->GetItem(li);
    long line = 0;
    li.m_text.ToLong(&line);
    cbEditor* ed = Manager::Get()->GetEditorManager()->Open(file);
    if (!line || !ed)
        return;

    line -= 1;
    ed->Activate();
    ed->GotoLine(line);

    if (cbStyledTextCtrl* ctrl = ed->GetControl()) {
        ctrl->EnsureVisible(line);
    }
}

void cbSearchResultsLog::OnDoubleClick(cb_unused wxCommandEvent& event)
{
    // go to the relevant file/line
    if (control->GetSelectedItemCount() == 0)
        return;

    // find selected item index
    int index = control->GetNextItem(-1,
                                     wxLIST_NEXT_ALL,
                                     wxLIST_STATE_SELECTED);

    SyncEditor(index);
} // end of OnDoubleClick
