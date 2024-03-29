/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 7109 $
* $Id: wxsstddialogbuttonsizer.h 7109 2011-04-15 11:53:16Z mortenmacfly $
* $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsstddialogbuttonsizer.h $
*/

#ifndef WXSSTDDIALOGBUTTONSIZER_H
#define WXSSTDDIALOGBUTTONSIZER_H

#include "../wxsitem.h"

class wxsStdDialogButtonSizer: public wxsItem
{
    public:

        wxsStdDialogButtonSizer(wxsItemResData* Data);

    private:

        virtual long OnGetPropertiesFlags();
        virtual void OnEnumItemProperties(long Flags);
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long PreviewFlags);
        virtual void OnBuildCreatingCode();
        virtual bool OnXmlRead(TiXmlElement* Element,bool IsXRC,bool IsExtra);
        virtual bool OnXmlWrite(TiXmlElement* Element,bool IsXRC,bool IsExtra);
        virtual void OnAddExtraProperties(wxsPropertyGridManager* Grid);
        virtual void OnExtraPropertyChanged(wxsPropertyGridManager* Grid,wxPGId Id);
        virtual void OnBuildDeclarationsCode();

        static const int NumButtons = 8;

        bool     m_Use    [NumButtons];
        wxPGId   m_UseId  [NumButtons];
        wxString m_Label  [NumButtons];
        wxPGId   m_LabelId[NumButtons];

};

#endif
