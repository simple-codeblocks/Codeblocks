//------------------------------------------------------------------------------
// Header generated by wxDesigner from file: stedlgs.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

#ifndef __WDR_stedlgs_H__
#define __WDR_stedlgs_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "stedlgs_wdr.h"
#endif

// Include wxWidgets' headers

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wx/statline.h>
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <wx/grid.h>
#include <wx/toolbar.h>
#include <wx/tglbtn.h>
#include <wx/collpane.h>
#include <wx/hyperlink.h>
#include <wx/animate.h>
#include <wx/filepicker.h>
#include <wx/datectrl.h>
#include <wx/fontpicker.h>
#include <wx/clrpicker.h>
#include <wx/srchctrl.h>
#if wxCHECK_VERSION(2,9,0)
#include <wx/editlbox.h>
#endif

// Declare window functions

#define ID_STEDLG_STYLE_NOTEBOOK 20000
#define ID_STEDLG_STYLELANG_CHOICE 20001
#define ID_LINE 20002
#define ID_STEDLG_FONT_CHECKBOX 20003
#define ID_STEDLG_FONT_BUTTON 20004
#define ID_STEDLG_FONT_CHOICE 20005
#define ID_STEDLG_FONTSIZE_CHECKBOX 20006
#define ID_STEDLG_FONTSIZE_SPINCTRL 20007
#define ID_STEDLG_ATTRIBUTES_CHECKBOX 20008
#define ID_STEDLG_BOLD_CHECKBOX 20009
#define ID_STEDLG_ITALICS_CHECKBOX 20010
#define ID_STEDLG_UNDERLINE_CHECKBOX 20011
#define ID_STEDLG_EOLFILL_CHECKBOX 20012
#define ID_STEDLG_FONTFORE_CHECKBOX 20013
#define ID_STEDLG_FONTFORE_BUTTON 20014
#define ID_STEDLG_FONTBACK_CHECKBOX 20015
#define ID_STEDLG_FONTBACK_BUTTON 20016
wxSizer *wxSTEditorStyleSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_TEXT 20017
#define ID_STEDLG_LANG_CHOICE 20018
#define ID_STEDLG_FILEPATTERN_TEXTCTRL 20019
#define ID_STEDLG_LANG_NOTEBOOK 20020
wxSizer *wxSTEditorLangSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEPROP_FILENAME_TEXTCTRL 20021
#define ID_STEPROP_FILESIZE_TEXT 20022
#define ID_STEPROP_FILEOPENED_TEXT 20023
#define ID_STEPROP_FILEMODIFIED_TEXT 20024
#define ID_STEPROP_FILEACCESSED_TEXT 20025
#define ID_STEPROP_FILECREATED_TEXT 20026
#define ID_STEPROP_FILE_TYPE_TEXT 20027
#define ID_STEPROP_LANGUAGE_TEXT 20028
#define ID_STEPROP_ENCODING_CHOICE 20029
#define ID_STEPROP_ENCODING_BOM_CHECKBOX 20030
#define ID_STEPROP_NUMLINES_TEXT 20031
#define ID_STEPROP_NUMCHARS_TEXT 20032
#define ID_STEPROP_NUMWORDS_TEXT 20033
#define ID_STEPROP_NUMTABS_TEXT 20034
#define ID_STEPROP_EOLCHARS_TEXT 20035
wxSizer *wxSTEditorPropertiesSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_FIND_COMBO 20036
#define ID_STEDLG_FIND_BITMAPBUTTON 20037
#define ID_STEDLG_REPLACE_TEXT 20038
#define ID_STEDLG_REPLACE_COMBO 20039
#define ID_STEDLG_REPLACE_BITMAPBUTTON 20040
#define ID_STEDLG_WHOLEWORD_CHECKBOX 20041
#define ID_STEDLG_BACKWARDS_CHECKBOX 20042
#define ID_STEDLG_MATCHCASE_CHECKBOX 20043
#define ID_STEDLG_WRAP_CHECKBOX 20044
#define ID_STEDLG_WORDSTART_CHECKBOX 20045
#define ID_STEDLG_REGEXP_FIND_CHECKBOX 20046
#define ID_STEDLG_FINDALL_CHECKBOX 20047
#define ID_STEDLG_BOOKMARKALL_CHECKBOX 20048
#define ID_STEDLG_SCOPEWHOLE_RADIOBUTTON 20049
#define ID_STEDLG_SCOPECURSOR_RADIOBUTTON 20050
#define ID_STEDLG_SCOPEALLDOCS_RADIOBUTTON 20051
#define ID_STEDLG_FIND_BUTTON 20052
#define ID_STEDLG_REPLACE_BUTTON 20053
#define ID_STEDLG_REPLACEFIND_BUTTON 20054
#define ID_STEDLG_REPLACEALL_BUTTON 20055
wxSizer *wxSTEditorFindReplaceSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_INSERT_PREPEND_RADIOBUTTON 10
#define ID_STEDLG_INSERT_APPEND_RADIOBUTTON 11
#define ID_STEDLG_INSERT_ATCOLUMN_RADIOBUTTON 12
#define ID_STEDLG_INSERT_COLUMN_SPINCTRL 20056
#define ID_STEDLG_INSERT_SURROUND_RADIOBUTTON 13
#define ID_STEDLG_INSERT_PREPEND_TEXT 20057
#define ID_STEDLG_INSERT_PREPEND_COMBO 20058
#define ID_STEDLG_INSERT_PREPEND_BITMAPBUTTON 20059
#define ID_STEDLG_INSERT_APPEND_COMBO 20060
#define ID_STEDLG_INSERT_APPEND_BITMAPBUTTON 20061
#define ID_STEDLG_INSERT_EDITOR 20062
wxSizer *wxSTEditorInsertTextSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_ZOOM_SPINCTRL 20063
#define ID_STEDLG_EDGE_MODE_CHOICE 20064
#define ID_STEDLG_EDGE_COLUMN_SPINCTRL 20065
#define ID_STEDLG_VIEW_LINEMARGIN_CHECKBOX 20066
#define ID_STEDLG_VIEW_MARKERMARGIN_CHECKBOX 20067
#define ID_STEDLG_CARET_LINE_VISIBLE_CHECKBOX 20068
#define ID_STEDLG_CARET_WIDTH_SPINCTRL 20069
#define ID_STEDLG_CARET_PERIOD_SPINCTRL 20070
wxSizer *wxSTEditorViewPrefsSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_PRINT_COLOURMODE_CHOICE 20071
#define ID_STEDLG_PRINT_MAGNIFICATION_SPINCTRL 20072
#define ID_STEDLG_PRINT_WRAPMODE_CHECKBOX 20073
#define ID_STEDLG_PRINT_LINENUMBERS_CHOICE 20074
wxSizer *wxSTEditorPrintPrefsSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_USE_TABS_CHECKBOX 20075
#define ID_STEDLG_TAB_WIDTH_SPINCTRL 20076
#define ID_STEDLG_TAB_INDENTS_CHECKBOX 20077
#define ID_STEDLG_INDENT_WIDTH_SPINCTRL 20078
#define ID_STEDLG_BACKSPACE_UNINDENTS_CHECKBOX 20079
#define ID_STEDLG_AUTOINDENT_CHECKBOX 20080
#define ID_STEDLG_INDENT_GUIDES_CHECKBOX 20081
#define ID_STEDLG_EOL_MODE_CHOICE 20082
#define ID_STEDLG_VIEW_EOL_CHECKBOX 20083
#define ID_STEDLG_VIEW_WHITESPACE_CHECKBOX 20084
wxSizer *wxSTEditorTabsPrefsSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_HIGHLIGHT_SYNTAX_CHECKBOX 20085
#define ID_STEDLG_HIGHLIGHT_PREPROC_CHECKBOX 20086
#define ID_STEDLG_HIGHLIGHT_BRACES_CHECKBOX 20087
#define ID_STEDLG_LOAD_INIT_LANG_CHECKBOX 20088
wxSizer *wxSTEditorHighlightingPrefsSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_VIEW_FOLDMARGIN_CHECKBOX 20089
#define ID_STEDLG_FOLD_STYLES_LISTBOX 20090
#define ID_STEDLG_FOLDMARGIN_STYLE_CHOICE 20091
#define ID_STEDLG_WRAP_MODE_CHECKBOX 20092
#define ID_STEDLG_WRAP_VISUALFLAGS_CHOICE 20093
#define ID_STEDLG_WRAP_VISUALFLAGSLOC_CHOICE 20094
#define ID_STEDLG_WRAP_STARTINDENT_SPINCTRL 20095
wxSizer *wxSTEditorFoldPrefsSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_WINDOWS_LISTBOX 20096
#define ID_STEDLGS_WINDOWS_ACTIVATE_BUTTON 20097
#define ID_STEDLGS_WINDOWS_SAVE_BUTTON 20098
#define ID_STEDLGS_WINDOWS_CLOSE_BUTTON 20099
wxSizer *wxSTEditorWindowsSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_COLUMNIZE_BEFORE_COMBO 20100
#define ID_STEDLG_COLUMNIZE_FORMAT_BUTTON 20101
#define ID_STEDLG_COLUMNIZE_AFTER_COMBO 20102
#define ID_STEDLG_COLUMNIZE_ORIGINAL_BUTTON 20103
#define ID_STEDLG_COLUMNIZE_PRESERVE_COMBO 20104
#define ID_STEDLG_COLUMNIZE_CHECKBOX 20105
#define ID_STEDLG_COLUMNIZE_IGNORE_COMBO 20106
#define ID_STEDLG_COLUMNIZE_EDITOR 20107
wxSizer *wxSTEditorColumnizeSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_EXPORT_FORMAT_CHOICE 20108
#define ID_STEDLG_EXPORT_FILENAME_COMBO 20109
#define ID_STEDLG_EXPORT_FILENAME_BITMAPBUTTON 20110
#define ID_STEDLG_EXPORT_EXTENSION_CHECKBOX 20111
wxSizer *wxSTEditorExportSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_LANG_KEYWORD_CHOICE 20112
#define ID_STEDLG_LANG_KEYWORD_TEXTCTRL 20113
#define ID_STEDLG_LANG_USERKEYWORD_TEXTCTRL 20114
wxSizer *wxSTEditorLangKeywordSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_LANG_STYLE_CHOICE 20115
#define ID_STEDLG_LANG_STYLE_EDITOR 20116
wxSizer *wxSTEditorLangStyleSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLG_LOAD_UNICODE_CHOICE 20117
#define ID_STEDLG_SAVE_WHITESPACE_CHECKBOX 20118
#define ID_STEDLG_SAVE_LINEENDINGS_CHECKBOX 20119
wxSizer *wxSTEditorLoadSavePrefsSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEFILEOPEN_ENCODING_CHOICE 20120
#define ID_STEFILEOPEN_ENCODING_BOM_CHECKBOX 20121
wxSizer *wxSTEditorFileOpenSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

#define ID_STEDLGS_BOOKMARKS_TREECTRL 20122
#define ID_STEDLGS_BOOKMARKS_GOTO_BUTTON 20123
#define ID_STEDLGS_BOOKMARKS_DELETE_BUTTON 20124
wxSizer *wxSTEditorBookmarkSizer( wxWindow *parent, bool call_fit = true, bool set_sizer = true );

// Declare menubar functions

// Declare toolbar functions

// Declare bitmap functions

wxBitmap wxSTEBitmapsFunc( size_t index );

#endif

// End of generated file
