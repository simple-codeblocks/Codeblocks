/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 9426 $
 * $Id: projectloader_hooks.cpp 9426 2013-11-02 19:42:20Z alpha0010 $
 * $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/sdk/projectloader_hooks.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "cbproject.h"
#endif

#include "projectloader_hooks.h"
#include "tinyxml.h"

#include <map>

namespace ProjectLoaderHooks
{
    typedef std::map<int, HookFunctorBase*> HookFunctorsMap;
    static HookFunctorsMap s_HookFunctorsMap;
    static int             s_UniqueID = 0;
}

int ProjectLoaderHooks::RegisterHook(ProjectLoaderHooks::HookFunctorBase* functor)
{
    for (HookFunctorsMap::iterator it = s_HookFunctorsMap.begin(); it != s_HookFunctorsMap.end(); ++it)
    {
        if (it->second == functor)
            return it->first;
    }
    s_HookFunctorsMap[s_UniqueID] = functor;
    return s_UniqueID++;
}

ProjectLoaderHooks::HookFunctorBase* ProjectLoaderHooks::UnregisterHook(int id, bool deleteHook)
{
    HookFunctorsMap::iterator it = s_HookFunctorsMap.find(id);
    if (it != s_HookFunctorsMap.end())
    {
        ProjectLoaderHooks::HookFunctorBase* functor = it->second;
        s_HookFunctorsMap.erase(it);
        if (deleteHook)
        {
            delete functor;
            return nullptr;
        }
        return functor;
    }
    return nullptr;
}

bool ProjectLoaderHooks::HasRegisteredHooks()
{
    return s_HookFunctorsMap.size() != 0;
}

void ProjectLoaderHooks::CallHooks(cbProject* project, TiXmlElement* elem, bool isLoading)
{
    for (HookFunctorsMap::iterator it = s_HookFunctorsMap.begin(); it != s_HookFunctorsMap.end(); ++it)
    {
        ProjectLoaderHooks::HookFunctorBase* functor = it->second;
        if (functor)
            functor->Call(project, elem, isLoading);
    }
}
