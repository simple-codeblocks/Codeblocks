/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ENVIRONMENTSETTINGSDLG_H
#define ENVIRONMENTSETTINGSDLG_H

#include <scrollingdialog.h>
#include <pluginmanager.h>

class wxAuiDockArt;
class wxListbookEvent;
class wxCheckListBox;

class EnvironmentSettingsDlg : public wxScrollingDialog
{
	public:
		EnvironmentSettingsDlg(wxWindow* parent, wxAuiDockArt* art);
		virtual ~EnvironmentSettingsDlg();
		virtual void EndModal(int retCode);
	protected:
        void OnPageChanging(wxListbookEvent& event);
        void OnPageChanged(wxListbookEvent& event);
        void OnSetAssocs(wxCommandEvent& event);
        void OnManageAssocs(wxCommandEvent& event);
        void OnChooseColour(wxCommandEvent& event);
        void OnResetDefaultColours(wxCommandEvent& event);
        void OnUseIpcCheck(wxCommandEvent& event);
        void OnPlaceCheck(wxCommandEvent& event);
        void OnHeadCheck(wxCommandEvent& event);
        void OnAutoHide(wxCommandEvent& event);
        void OnI18NCheck(wxCommandEvent& event);
        void OnSettingsIconsSize(wxCommandEvent& event);
        void OnDblClickMaximizes(wxCommandEvent& event);
        void OnMousewheelModifier(wxKeyEvent& event);
        void OnUseTabMousewheel(wxCommandEvent& event);

        void OnChooseAppColourCategory(wxCommandEvent &event);
        void OnChooseAppColourItem(wxCommandEvent &event);
        void OnClickAppColour(wxCommandEvent &event);
	private:
        void AddPluginPanels();
        void LoadListbookImages();
        void UpdateListbookImages();

        void FillApplicationColours();
        void WriteApplicationColours();
        wxString AnnoyingDlgReturnToString(const wxString& caption);
        wxString StringToAnnoyingDlgReturn(const wxString& caption);
    private:
        wxAuiDockArt* m_pArt;
        ConfigurationPanelsArray m_PluginPanels;
        std::map<wxString, wxColour> m_ChangedAppColours;
        std::map<wxString, wxString> m_AnnoyingDlgReturnMap;
        wxImageList* m_pImageList;

        DECLARE_EVENT_TABLE()
};



#endif // ENVIRONMENTSETTINGSDLG_H

