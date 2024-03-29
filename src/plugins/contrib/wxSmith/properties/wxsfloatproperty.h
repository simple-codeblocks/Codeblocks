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
* $Revision: 8148 $
* $Id: wxsfloatproperty.h 8148 2012-07-19 19:50:28Z mortenmacfly $
* $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/properties/wxsfloatproperty.h $
*/

#ifndef WXSFLOATPROPERTY_H
#define WXSFLOATPROPERTY_H

#include "wxsproperty.h"

/** \brief Base float property */
class wxsFloatProperty: public wxsProperty
{
    public:

        /** \brief Ctor
         *  \param PGName   name of property in Property Grid
         *  \param DataName name of property in data stuctures
         *  \param Offset   offset of value (returned from wxsOFFSET macro)
         *  \param Default  default value applied on read errors
         *  \param Priority priority of this property
         */
        wxsFloatProperty(const wxString& PGName,const wxString& DataName,long Offset,double Default=0.0f,int Priority=100);

        /** \brief Returning type name */
        virtual const wxString GetTypeName() { return _T("float"); }

    protected:

        virtual void PGCreate(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Parent);
        virtual bool PGRead(wxsPropertyContainer* Object,wxPropertyGridManager* Grid, wxPGId Id,long Index);
        virtual bool PGWrite(wxsPropertyContainer* Object,wxPropertyGridManager* Grid, wxPGId Id,long Index);
        virtual bool XmlRead(wxsPropertyContainer* Object,TiXmlElement* Element);
        virtual bool XmlWrite(wxsPropertyContainer* Object,TiXmlElement* Element);
        virtual bool PropStreamRead(wxsPropertyContainer* Object,wxsPropertyStream* Stream);
        virtual bool PropStreamWrite(wxsPropertyContainer* Object,wxsPropertyStream* Stream);

    private:
        long Offset;
        double Default;
};

/** \addtogroup properties_macros
 *  \{ */

/** \brief Macro automatically declaring float property
 *  \param ClassName name of class holding this property
 *  \param VarName name of variable inside class
 *  \param PGName name used in property grid
 *  \param DataName name used in Xml / Data Streams
 *  \param Default value applied on read errors / validation failures
 */
#define WXS_FLOAT(ClassName,VarName,PGName,DataName,Default) \
    { static wxsFloatProperty _Property(PGName,DataName,wxsOFFSET(ClassName,VarName),Default); \
      Property(_Property); }

/** \brief Macro automatically declaring float property with custom priority
 *  \param ClassName name of class holding this property
 *  \param VarName name of variable inside class
 *  \param PGName name used in property grid
 *  \param DataName name used in Xml / Data Streams
 *  \param Default value applied on read errors / validation failures
 */
#define WXS_FLOAT_P(ClassName,VarName,PGName,DataName,Default,Priority) \
    { static wxsFloatProperty _Property(PGName,DataName,wxsOFFSET(ClassName,VarName),Default,Priority); \
      Property(_Property); }

/** \} */

#endif // WXSFLOATPROPERTY_H
