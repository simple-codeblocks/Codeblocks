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
* $Revision: 8704 $
* $Id: wxsfloatproperty.cpp 8704 2012-12-23 20:32:03Z mortenmacfly $
* $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/properties/wxsfloatproperty.cpp $
*/

#include "wxsfloatproperty.h"
#include <globals.h>

// Helper macro for fetching variable
#define VALUE   wxsVARIABLE(Object,Offset,double)

wxsFloatProperty::wxsFloatProperty(const wxString& PGName, const wxString& DataName,long _Offset,double _Default,int Priority):
    wxsProperty(PGName,DataName,Priority),
    Offset(_Offset),
    Default(_Default)
{}


void wxsFloatProperty::PGCreate(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Parent)
{
    PGRegister(Object,Grid,Grid->AppendIn(Parent,NEW_IN_WXPG14X wxFloatProperty(GetPGName(),wxPG_LABEL,VALUE)));
}

bool wxsFloatProperty::PGRead(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,
                              wxPGId Id, cb_unused long Index)
{
    VALUE = Grid->GetPropertyValue(Id).GetDouble();
    return true;
}

bool wxsFloatProperty::PGWrite(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,
                               wxPGId Id, cb_unused long Index)
{
    Grid->SetPropertyValue(Id,VALUE);
    return true;
}

bool wxsFloatProperty::XmlRead(wxsPropertyContainer* Object,TiXmlElement* Element)
{
    if ( !Element )
    {
        VALUE = Default;
        return false;
    }
    const char* Text = Element->GetText();
    if ( !Text )
    {
        VALUE = Default;
        return false;
    }
    VALUE = atof(Text);
    return true;
}

bool wxsFloatProperty::XmlWrite(wxsPropertyContainer* Object,TiXmlElement* Element)
{
    if ( VALUE != Default )
    {
        Element->InsertEndChild(TiXmlText(cbU2C(wxString::Format(_T("%lf"),VALUE))));
        return true;
    }
    return false;
}

bool wxsFloatProperty::PropStreamRead(wxsPropertyContainer* Object,wxsPropertyStream* Stream)
{
    return Stream->GetDouble(GetDataName(), VALUE, Default);
}

bool wxsFloatProperty::PropStreamWrite(wxsPropertyContainer* Object,wxsPropertyStream* Stream)
{
    return Stream->PutDouble(GetDataName(),VALUE,Default);
}
