/***************************************************************
 * Name:      ThreadSearchConfPanel
 * Purpose:   This class implements the configuration panel used
 *            in modal dialog called on "Options" button click
 *            and by C::B on "Environment" settings window.
 * Author:    Jerome ANTOINE
 * Created:   2007-10-08
 * Copyright: Jerome ANTOINE
 * License:   GPL
 **************************************************************/

#if defined(CB_PRECOMP)
#include "sdk.h"
#endif
//#include <sdk.h> // Code::Blocks SDK
#ifndef CB_PRECOMP
	#include <wx/checkbox.h>
	#include <wx/radiobox.h>
	#include <wx/sizer.h>
	#include <wx/statbox.h>
	#include <wx/stattext.h>
#endif

#include "ThreadSearch.h"
#include "SearchInPanel.h"
#include "DirectoryParamsPanel.h"
#include "ThreadSearchConfPanel.h"
#include "ThreadSearchControlIds.h"
#include "ThreadSearchViewManagerBase.h"
#include "ThreadSearchFindData.h"
#include "InsertIndexManager.h"

// begin wxGlade: ::extracode

// end wxGlade


ThreadSearchConfPanel::ThreadSearchConfPanel(ThreadSearch& threadSearchPlugin, wxWindow* parent,wxWindowID id)
					  :m_ThreadSearchPlugin(threadSearchPlugin)
{
    Create(parent,id,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);

    // begin wxGlade: ThreadSearchConfPanel::ThreadSearchConfPanel
    SizerOptions_staticbox = new wxStaticBox(this, -1, wxT("Options"));
    SizerThreadSearchOptions_staticbox = new wxStaticBox(this, -1, wxT("Snippets search options"));
    SizerThreadSearchLayoutGlobal_staticbox = new wxStaticBox(this, -1, wxT("Show/Hide"));
    SizerListControlOptions_staticbox = new wxStaticBox(this, -1, wxT("List control options"));
    SizerThreadSearchLayout_staticbox = new wxStaticBox(this, -1, wxT("Layout"));
    SizerSearchIn_staticbox = new wxStaticBox(this, -1, wxT("Search in"));
    m_pPnlSearchIn = new SearchInPanel(this, wxID_ANY);
    m_pPnlDirParams = new DirectoryParamsPanel(this, wxID_ANY);
    m_pChkWholeWord = new wxCheckBox(this, idChkWholeWord, wxT("Whole word"));
    m_pChkStartWord = new wxCheckBox(this, idChkStartWord, wxT("Start word"));
    m_pChkMatchCase = new wxCheckBox(this, idChkMatchCase, wxT("Match case"));
    m_pChkRegExp = new wxCheckBox(this, idChkRegularExpression, wxT("Regular expression"));
    m_pChkThreadSearchEnable = new wxCheckBox(this, idChkThreadSearchEnable, wxT("Enable 'Find occurrences' contextual menu item"));
    m_pChkUseDefaultOptionsForThreadSearch = new wxCheckBox(this, idChkUseDefaultOptionsOnThreadSearch, wxT("Use default options when running 'Find occurrences' "));
    //-moved-below-m_pChkShowThreadSearchToolBar = new wxCheckBox(this, idChkViewThreadSearchToolBar, wxT("Show ThreadSearch toolbar."));
    m_pChkShowThreadSearchWidgets = new wxCheckBox(this, idChkShowThreadSearchWidgets, wxT("Show search widgets in ThreadSearch Messages panel."));
    m_pChkShowCodePreview = new wxCheckBox(this, idChkShowCodePreview, wxT("Show code preview editor."));
    m_pChkShowThreadSearchToolBar = new wxCheckBox(this, idChkViewThreadSearchToolBar, wxT("Show ThreadSearch toolbar."));
    m_pChkDisplayLogHeaders = new wxCheckBox(this, idChkDisplayLogHeaders, wxT("Display header in log window."));
    m_pChkDrawLogLines = new wxCheckBox(this, idChkDrawLogLines, wxT("Draw lines between log columns."));

    const wxString m_pRadPanelManagement_choices[] = {
        wxT("Messages notebook"),
        wxT("Layout")
    };
    m_pRadPanelManagement = new wxRadioBox(this, wxID_ANY, wxT("SnippetsSearch panel management by"), wxDefaultPosition, wxDefaultSize, 2, m_pRadPanelManagement_choices, 1, wxRA_SPECIFY_ROWS);

    const wxString m_pRadLoggerType_choices[] = {
        wxT("List"),
        wxT("Tree")
    };
    m_pRadLoggerType = new wxRadioBox(this, wxID_ANY, wxT("Logger type"), wxDefaultPosition, wxDefaultSize, 2, m_pRadLoggerType_choices, 1, wxRA_SPECIFY_ROWS);

    const wxString m_pRadSplitterWndMode_choices[] = {
        wxT("Horizontal"),
        wxT("Vertical")
    };
    m_pRadSplitterWndMode = new wxRadioBox(this, wxID_ANY, wxT("Splitter window mode"), wxDefaultPosition, wxDefaultSize, 2, m_pRadSplitterWndMode_choices, 1, wxRA_SPECIFY_ROWS);

    const wxString m_pRadSortBy_choices[] = {
        wxT("file path"),
        wxT("file name")
    };
    m_pRadSortBy = new wxRadioBox(this, wxID_ANY, wxT("Sort results by"), wxDefaultPosition, wxDefaultSize, 2, m_pRadSortBy_choices, 1, wxRA_SPECIFY_ROWS);

    set_properties();
    do_layout();
    // end wxGlade

    // CodeSnippets does not show the SearchToolBar or use Layout options
    m_pChkShowThreadSearchToolBar->Show(false); //(pecan 2008/5/22)
    m_pRadPanelManagement->Show(false);         //(pecan 2008/5/22)
    // Not yet supported
    //SizerThreadSearchOptions_staticbox->Show(false);     //(pecan 2008/5/22)
    m_pChkThreadSearchEnable->Show(false);               //(pecan 2008/5/22)
    m_pChkUseDefaultOptionsForThreadSearch->Show(false); //(pecan 2008/5/22)
}


BEGIN_EVENT_TABLE(ThreadSearchConfPanel, wxPanel)
    // begin wxGlade: ThreadSearchConfPanel::event_table
    EVT_CHECKBOX(idChkThreadSearchEnable, ThreadSearchConfPanel::OnThreadSearchEnable)
    EVT_CHECKBOX(idChkViewThreadSearchToolBar, ThreadSearchConfPanel::OnChkShowThreadSearchToolBarClick)
    EVT_CHECKBOX(idChkShowCodePreview, ThreadSearchConfPanel::OnChkCodePreview)
    EVT_CHECKBOX(idChkShowThreadSearchWidgets, ThreadSearchConfPanel::OnChkShowThreadSearchWidgetsClick)
    // end wxGlade
END_EVENT_TABLE();


void ThreadSearchConfPanel::OnThreadSearchEnable(wxCommandEvent &event)
{
	m_pChkUseDefaultOptionsForThreadSearch->Enable(event.IsChecked());
    event.Skip();
}


void ThreadSearchConfPanel::OnChkShowThreadSearchToolBarClick(wxCommandEvent &event)
{
	if ( (event.IsChecked() == false) && (m_pChkShowThreadSearchWidgets->GetValue() == false) )
	{
		if ( wxID_YES != cbMessageBox(wxT("Do you really want to hide both ThreadSearch toolbar and widgets ?"),
									  wxT("Sure ?"), wxICON_QUESTION|wxYES_NO) )
		{
			m_pChkShowThreadSearchToolBar->SetValue(true);
		}
	}
	event.Skip();
}


void ThreadSearchConfPanel::OnChkCodePreview(wxCommandEvent &event)
{
	m_pRadSplitterWndMode->Enable(event.IsChecked());
	event.Skip();
}


void ThreadSearchConfPanel::OnChkShowThreadSearchWidgetsClick(wxCommandEvent &event)
{
	if ( (event.IsChecked() == false) && (m_pChkShowThreadSearchToolBar->GetValue() == false) )
	{
		if ( wxID_YES != cbMessageBox(wxT("Do you really want to hide both ThreadSearch toolbar and widgets ?"),
									  wxT("Sure ?"), wxICON_QUESTION|wxYES_NO) )
		{
			m_pChkShowThreadSearchWidgets->SetValue(true);
		}
	}
	event.Skip();
}


// wxGlade: add ThreadSearchConfPanel event handlers


void ThreadSearchConfPanel::set_properties()
{
    // begin wxGlade: ThreadSearchConfPanel::set_properties
    m_pChkWholeWord->SetToolTip(wxT("Search text matches only whole words"));
    m_pChkWholeWord->SetValue(1);
    m_pChkStartWord->SetToolTip(wxT("Matches only word starting with search expression"));
    m_pChkMatchCase->SetToolTip(wxT("Case sensitive search."));
    m_pChkMatchCase->SetValue(1);
    m_pChkRegExp->SetToolTip(wxT("Search expression is a regular expression"));
    m_pChkThreadSearchEnable->SetValue(1);
    m_pChkUseDefaultOptionsForThreadSearch->SetValue(1);
    m_pChkShowThreadSearchToolBar->SetValue(1);
    m_pChkShowThreadSearchWidgets->SetValue(1);
    m_pChkShowCodePreview->SetValue(1);
    m_pChkDisplayLogHeaders->SetValue(1);
    m_pRadPanelManagement->SetSelection(0);
    m_pRadLoggerType->SetSelection(0);
    m_pRadSplitterWndMode->SetSelection(0);
    m_pRadSortBy->SetSelection(0);
    // end wxGlade

	ThreadSearchFindData findData;
	m_ThreadSearchPlugin.GetFindData(findData);

    m_pChkWholeWord->SetValue(findData.GetMatchWord());
    m_pChkStartWord->SetValue(findData.GetStartWord());
    m_pChkMatchCase->SetValue(findData.GetMatchCase());
    m_pChkRegExp->SetValue(findData.GetRegEx());
    m_pChkThreadSearchEnable->SetValue(m_ThreadSearchPlugin.GetCtxMenuIntegration());
    m_pChkUseDefaultOptionsForThreadSearch->SetValue(m_ThreadSearchPlugin.GetUseDefValsForThreadSearch());
    m_pChkUseDefaultOptionsForThreadSearch->Enable(m_ThreadSearchPlugin.GetCtxMenuIntegration());
    m_pChkDisplayLogHeaders->SetValue(m_ThreadSearchPlugin.GetDisplayLogHeaders());
    m_pChkDrawLogLines->SetValue(m_ThreadSearchPlugin.GetDrawLogLines());
    m_pChkShowThreadSearchToolBar->SetValue(m_ThreadSearchPlugin.IsToolbarVisible());
    m_pChkShowThreadSearchWidgets->SetValue(m_ThreadSearchPlugin.GetShowSearchControls());
    m_pChkShowCodePreview->SetValue(m_ThreadSearchPlugin.GetShowCodePreview());

    int radIndex = 0;
    switch (m_ThreadSearchPlugin.GetManagerType())
    {
		case ThreadSearchViewManagerBase::TypeLayout :
		{
			radIndex = 1;
			break;
		}
		case ThreadSearchViewManagerBase::TypeMessagesNotebook : // fall-though
		default:
		{
			radIndex = 0;
			break;
		}
    }
    m_pRadPanelManagement->SetSelection(radIndex);

    radIndex = 0;
    switch (m_ThreadSearchPlugin.GetLoggerType())
    {
		case ThreadSearchLoggerBase::TypeTree:
		{
			radIndex = 1;
			break;
		}
		case ThreadSearchLoggerBase::TypeList : // fall-though
		default:
		{
			radIndex = 0;
			break;
		}
    }
    m_pRadLoggerType->SetSelection(radIndex);

    radIndex = 0;
    switch (m_ThreadSearchPlugin.GetSplitterMode())
    {
		case wxSPLIT_HORIZONTAL:
		{
			radIndex = 0;
			break;
		}
		default :
		// case wxSPLIT_VERTICAL:
		{
			radIndex = 1;
			break;
		}
    }
    m_pRadSplitterWndMode->SetSelection(radIndex);
    m_pRadSplitterWndMode->Enable(m_ThreadSearchPlugin.GetShowCodePreview());

    radIndex = 0;
    switch (m_ThreadSearchPlugin.GetFileSorting())
    {
		case InsertIndexManager::SortByFileName:
		{
			radIndex = 1;
			break;
		}
		case InsertIndexManager::SortByFilePath : // fall-through
		default:
		{
			radIndex = 0;
			break;
		}
    }
    m_pRadSortBy->SetSelection(radIndex);

    m_pPnlDirParams->SetSearchDirHidden(findData.GetHiddenSearch());
    m_pPnlDirParams->SetSearchDirRecursively(findData.GetRecursiveSearch());
    m_pPnlDirParams->SetSearchDirPath(findData.GetSearchPath());
    m_pPnlDirParams->SetSearchMask(findData.GetSearchMask());

    m_pPnlSearchIn->SetSearchInOpenFiles(findData.MustSearchInOpenFiles());
    m_pPnlSearchIn->SetSearchInSnippetFiles(findData.MustSearchInProject());
    m_pPnlSearchIn->SetSearchInSnippetFiles(findData.MustSearchInCodeSnippetsTree());
    //-m_pPnlSearchIn->SetSearchInWorkspaceFiles(findData.MustSearchInWorkspace());
    m_pPnlSearchIn->SetSearchInDirectory(findData.MustSearchInDirectory());
}


void ThreadSearchConfPanel::do_layout()
{
#if wxCHECK_VERSION(2, 9, 0)
    #define wxADJUST_MINSIZE 0
#endif
    // begin wxGlade: ThreadSearchConfPanel::do_layout
    wxBoxSizer* SizerTop = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* SizerThreadSearchLayout = new wxStaticBoxSizer(SizerThreadSearchLayout_staticbox, wxVERTICAL);
    wxFlexGridSizer* SizerThreadSearchGridLayout = new wxFlexGridSizer(4, 2, 0, 0);
    wxStaticBoxSizer* SizerListControlOptions = new wxStaticBoxSizer(SizerListControlOptions_staticbox, wxVERTICAL);
    wxStaticBoxSizer* SizerThreadSearchLayoutGlobal = new wxStaticBoxSizer(SizerThreadSearchLayoutGlobal_staticbox, wxVERTICAL);
    wxStaticBoxSizer* SizerThreadSearchOptions = new wxStaticBoxSizer(SizerThreadSearchOptions_staticbox, wxVERTICAL);
    wxStaticBoxSizer* SizerOptions = new wxStaticBoxSizer(SizerOptions_staticbox, wxHORIZONTAL);
    wxStaticBoxSizer* SizerSearchIn = new wxStaticBoxSizer(SizerSearchIn_staticbox, wxVERTICAL);
    SizerSearchIn->Add(m_pPnlSearchIn, 0, wxALL|wxEXPAND, 2);
    SizerSearchIn->Add(m_pPnlDirParams, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 2);
    SizerTop->Add(SizerSearchIn, 0, wxALL|wxEXPAND, 4);
    SizerOptions->Add(m_pChkWholeWord, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerOptions->Add(m_pChkStartWord, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerOptions->Add(m_pChkMatchCase, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerOptions->Add(m_pChkRegExp, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerTop->Add(SizerOptions, 0, wxALL|wxEXPAND, 4);
    SizerThreadSearchOptions->Add(m_pChkThreadSearchEnable, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerThreadSearchOptions->Add(m_pChkUseDefaultOptionsForThreadSearch, 0, wxALL|wxADJUST_MINSIZE, 4);
    //(pecan 2008/5/22) not supported yet
    //wxStaticText* m_pStaDefaultOptions = new wxStaticText(this, wxID_ANY, wxT("       ('Whole word' = true, 'Start word' = false, 'Match case' = true, 'Regular expression' = false)"));
    wxStaticText* m_pStaDefaultOptions = new wxStaticText(this, wxID_ANY, wxT(" Place holder for yet to be supported context menu option."));
    SizerThreadSearchOptions->Add(m_pStaDefaultOptions, 0, wxADJUST_MINSIZE, 0);
    SizerTop->Add(SizerThreadSearchOptions, 0, wxALL|wxEXPAND, 4);
    //--moved--belo--SizerThreadSearchLayoutGlobal->Add(m_pChkShowThreadSearchToolBar, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerThreadSearchLayoutGlobal->Add(m_pChkShowThreadSearchWidgets, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerThreadSearchLayoutGlobal->Add(m_pChkShowCodePreview, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerThreadSearchLayoutGlobal->Add(m_pChkShowThreadSearchToolBar, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerThreadSearchGridLayout->Add(SizerThreadSearchLayoutGlobal, 1, wxALL|wxEXPAND|wxADJUST_MINSIZE, 4);
    SizerListControlOptions->Add(m_pChkDisplayLogHeaders, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerListControlOptions->Add(m_pChkDrawLogLines, 0, wxALL|wxADJUST_MINSIZE, 4);
    SizerThreadSearchGridLayout->Add(SizerListControlOptions, 1, wxALL|wxEXPAND|wxADJUST_MINSIZE, 4);
    SizerThreadSearchGridLayout->Add(m_pRadPanelManagement, 0, wxALL|wxEXPAND|wxADJUST_MINSIZE, 4);
    SizerThreadSearchGridLayout->Add(m_pRadLoggerType, 0, wxALL|wxEXPAND|wxADJUST_MINSIZE, 4);
    SizerThreadSearchGridLayout->Add(m_pRadSplitterWndMode, 0, wxALL|wxEXPAND, 4);
    SizerThreadSearchGridLayout->Add(m_pRadSortBy, 0, wxALL|wxEXPAND, 4);
    SizerThreadSearchGridLayout->AddGrowableCol(0);
    SizerThreadSearchGridLayout->AddGrowableCol(1);
    SizerThreadSearchLayout->Add(SizerThreadSearchGridLayout, 1, wxALL|wxEXPAND|wxADJUST_MINSIZE, 0);
    SizerTop->Add(SizerThreadSearchLayout, 0, wxALL|wxEXPAND, 4);
    SetAutoLayout(true);
    SetSizer(SizerTop);
    SizerTop->Fit(this);
    SizerTop->SetSizeHints(this);
    // end wxGlade
}


void ThreadSearchConfPanel::OnApply()
{
	ThreadSearchFindData findData;

	// Inits variable data from user values
	findData.SetFindText       (wxEmptyString);
	findData.SetHiddenSearch   (m_pPnlDirParams->GetSearchDirHidden());
	findData.SetRecursiveSearch(m_pPnlDirParams->GetSearchDirRecursively());
	findData.SetSearchPath     (m_pPnlDirParams->GetSearchDirPath());
	findData.SetSearchMask     (m_pPnlDirParams->GetSearchMask());
	findData.SetMatchWord      (m_pChkWholeWord->IsChecked());
	findData.SetStartWord      (m_pChkStartWord->IsChecked());
	findData.SetMatchCase      (m_pChkMatchCase->IsChecked());
	findData.SetRegEx          (m_pChkRegExp->IsChecked());

	findData.UpdateSearchScope(ScopeOpenFiles,      m_pPnlSearchIn->GetSearchInOpenFiles());
	findData.UpdateSearchScope(ScopeSnippetFiles,   m_pPnlSearchIn->GetSearchInSnippetFiles());
	//-findData.UpdateSearchScope(ScopeWorkspaceFiles, m_pPnlSearchIn->GetSearchInWorkspaceFiles());
	findData.UpdateSearchScope(ScopeDirectoryFiles, m_pPnlSearchIn->GetSearchInDirectory());

	// Updates plug-in with new datas
	m_ThreadSearchPlugin.SetFindData(findData);
	m_ThreadSearchPlugin.SetCtxMenuIntegration(m_pChkThreadSearchEnable->IsChecked());
	m_ThreadSearchPlugin.SetUseDefValsForThreadSearch(m_pChkUseDefaultOptionsForThreadSearch->IsChecked());
	m_ThreadSearchPlugin.SetShowSearchControls(m_pChkShowThreadSearchWidgets->IsChecked());
	m_ThreadSearchPlugin.SetShowCodePreview(m_pChkShowCodePreview->IsChecked());
	m_ThreadSearchPlugin.SetDisplayLogHeaders(m_pChkDisplayLogHeaders->IsChecked());
	m_ThreadSearchPlugin.SetDrawLogLines(m_pChkDrawLogLines->IsChecked());

    int radIndex = m_pRadPanelManagement->GetSelection();
    ThreadSearchViewManagerBase::eManagerTypes mgrType;
    switch (radIndex)
    {
		case 1 :
		{
			mgrType = ThreadSearchViewManagerBase::TypeLayout;
			break;
		}
		default:
		// case 0 :
		{
			mgrType = ThreadSearchViewManagerBase::TypeMessagesNotebook;
			break;
		}
    }
    m_ThreadSearchPlugin.SetManagerType(mgrType);

    radIndex = m_pRadLoggerType->GetSelection();
    ThreadSearchLoggerBase::eLoggerTypes lgrType;
    switch (radIndex)
    {
		case 1 :
		{
			lgrType = ThreadSearchLoggerBase::TypeTree;
			break;
		}
		default:
		// case 0 :
		{
			lgrType = ThreadSearchLoggerBase::TypeList;
			break;
		}
    }
    m_ThreadSearchPlugin.SetLoggerType(lgrType);

    radIndex = m_pRadSortBy->GetSelection();
    InsertIndexManager::eFileSorting sortingType;
    switch (radIndex)
    {
		case 1 :
		{
			sortingType = InsertIndexManager::SortByFileName;
			break;
		}
		default:
		// case 0 :
		{
			sortingType = InsertIndexManager::SortByFilePath;
			break;
		}
    }
    m_ThreadSearchPlugin.SetFileSorting(sortingType);

    radIndex = m_pRadSplitterWndMode->GetSelection();
    wxSplitMode splitterMode;
    switch (radIndex)
    {
		case 1 :
		{
			splitterMode = wxSPLIT_VERTICAL;
			break;
		}
		default:
		// case 0 :
		{
			splitterMode = wxSPLIT_HORIZONTAL;
			break;
		}
    }
    m_ThreadSearchPlugin.SetSplitterMode(splitterMode);

	// Updates toolbar visibility
	m_ThreadSearchPlugin.ShowToolBar(m_pChkShowThreadSearchToolBar->IsChecked());

	// Notifies plug-in observers (ThreadSearchView)
	m_ThreadSearchPlugin.Notify();
}

