/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 7608 $
 * $Id: configmanager-revision.cpp 7608 2011-11-25 20:50:28Z mortenmacfly $
 * $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/sdk/configmanager-revision.cpp $
 */

/* ------------------------------------------------------------------------------------------------------------------
*  Decouple the autorevisioning code, so we don't need to recompile the whole manager each time.
*  It's bad enough we have to re-link the SDK...
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "configmanager.h"
    #include <wx/string.h>
#endif

#include "autorevision.h"

wxString ConfigManager::GetRevisionString()
{
    // Code::Blocks needs wxWidgets 2.8
    CompileTimeAssertion<wxMinimumVersion<2,8>::eval>::Assert();

    return autorevision::svnRevision;
}

unsigned int ConfigManager::GetRevisionNumber()
{
    return autorevision::svn_revision;
}

wxString ConfigManager::GetSvnDate()
{
    return autorevision::svnDate;
}

