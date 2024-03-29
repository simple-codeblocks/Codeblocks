#include "DictionariesNeededDialog.h"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>

DictionariesNeededDialog::DictionariesNeededDialog():
AnnoyingDialog(_("Dictionaries needed!"),_("SpellChecker needs a dictionary to work correctly.\nThe following page explains where to get\nand how to install a dictionary:"), wxART_INFORMATION, OK)
{
    wxSizer *outerSizer = GetSizer();

    // assuming annoying dialog is disabled if no sizer was returned here.
    if ( outerSizer )
    {
        wxSizerItemList& childs = outerSizer->GetChildren();
        wxSizer *mainArea = childs[0]->GetSizer();

        wxHyperlinkCtrl *hyperlink1 = new wxHyperlinkCtrl( this, wxID_ANY, _("How to configure SpellChecker?"), _T("http://wiki.codeblocks.org/index.php?title=SpellChecker_plugin"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );

        mainArea->Add(hyperlink1, 0, wxALL, 5);

        this->Layout();
        outerSizer->Fit(this);
        this->Centre( wxBOTH );
    }
}

DictionariesNeededDialog::~DictionariesNeededDialog()
{
}

