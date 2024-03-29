/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 8680 $
 * $Id: msvcworkspacebase.cpp 8680 2012-12-16 14:58:35Z mortenmacfly $
 * $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/projectsimporter/msvcworkspacebase.cpp $
 */

#include "sdk.h"

#ifndef CB_PRECOMP
    #include "manager.h"
    #include "logmanager.h"
    #include "projectmanager.h"
    #include "compilerfactory.h"
    #include "compiler.h"
    #include "cbproject.h"
#endif

#include "msvcworkspacebase.h"

MSVCWorkspaceBase::MSVCWorkspaceBase()
{
    // nothing to do
}

MSVCWorkspaceBase::~MSVCWorkspaceBase()
{
    // nothing to do
}

void MSVCWorkspaceBase::registerProject(const wxString& projectID, cbProject* project)
{
    // just set the initial project dependencies as empty and register the idcode
    Manager::Get()->GetLogManager()->DebugLog(wxString::Format(_T("MSVC import: registered project uuid=%s"), projectID.c_str()));
    _projects[projectID.Lower()] = ProjectRecord(project);
}

void MSVCWorkspaceBase::addDependency(const wxString& projectID, const wxString& dependencyID)
{
    // add the dependency to the last project
    Manager::Get()->GetLogManager()->DebugLog(wxString::Format(_T("MSVC import: Add dependency uuid=%s, key[1]=%s"), projectID.c_str(), dependencyID.c_str()));

    HashProjects::iterator it = _projects.find(projectID.Lower());
    if (it != _projects.end())
    {
        if (it->second._dependencyList.Index(dependencyID.Lower()) == wxNOT_FOUND) // not already in
            it->second._dependencyList.Add(dependencyID.Lower());
    }
    else
        Manager::Get()->GetLogManager()->DebugLog(_T("ERROR: project id not found: ") + projectID);
}

void MSVCWorkspaceBase::addWorkspaceConfiguration(const wxString& config)
{
    _workspaceConfigurations.Add(config);
}

void MSVCWorkspaceBase::addConfigurationMatching(const wxString& projectID, const wxString& workspConfig, const wxString& projConfig)
{
    //Manager::Get()->GetLogManager()->DebugLog(_T("adding conf match: '%s' - '%s'"), workspConfig.c_str(), projConfig.c_str());
    HashProjects::iterator it = _projects.find(projectID);
    if (it != _projects.end())
        it->second._configurations[workspConfig] = projConfig;
    else
        Manager::Get()->GetLogManager()->DebugLog(_T("ERROR: project id not found: ") + projectID);
}

void MSVCWorkspaceBase::updateProjects()
{
    HashProjects::iterator projIt;
    HashProjects::iterator depIt;
    ProjectBuildTarget* targetProj = 0;
    ProjectBuildTarget* targetDep = 0;
    ProjectRecord proj;
    ProjectRecord dep;
    unsigned int i;
    unsigned int j;
    int k;

    Manager::Get()->GetLogManager()->DebugLog(_T("Update projects"));

    // no per-workspace config for msvc6, so build a fake one ;)
    if (_workspaceConfigurations.IsEmpty())
    {
        for (projIt = _projects.begin(); projIt != _projects.end(); ++projIt)
        {
            proj = projIt->second;
            for (k=0; k<proj._project->GetBuildTargetsCount(); ++k)
            {
                // should be the configurations, not the build target title
                wxString s = proj._project->GetBuildTarget(k)->GetTitle();
                if (_workspaceConfigurations.Index(s) == wxNOT_FOUND)
                {
                    _workspaceConfigurations.Add(s);
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("workspace config: '%s'"), s.wx_str()));
                }
            }
        }
    }

    for (projIt = _projects.begin(); projIt != _projects.end(); ++projIt)
    {
        proj = projIt->second;
        Manager::Get()->GetLogManager()->DebugLog(F(_T("Project %s, %lu dependencies"), proj._project->GetTitle().wx_str(), static_cast<unsigned long>(proj._dependencyList.GetCount())));
        for (i=0; i<proj._dependencyList.GetCount(); ++i)
        {
            depIt = _projects.find(proj._dependencyList[i]);
            if ( depIt != _projects.end()) // dependency found
            {
                dep = depIt->second;

                Manager::Get()->GetProjectManager()->AddProjectDependency(proj._project, dep._project);

                // match target configurations
                for (j=0; j<_workspaceConfigurations.GetCount(); ++j)
                {
                    ConfigurationMatchings::iterator configIt;
                    wxString wconfig;
                    wxString pconfig;
                    targetProj = 0;
                    targetDep = 0;

                    if (proj._configurations.empty()) // msvc6
                    {
                        wconfig = _workspaceConfigurations[j];
                        targetProj = proj._project->GetBuildTarget(wconfig);
                        if (targetProj == 0)
                        {
                            // look for a project config which is a substring of the workspace config
                            for (int l=0; l<proj._project->GetBuildTargetsCount(); ++l)
                            {
                                pconfig = proj._project->GetBuildTarget(l)->GetTitle();
                                //Manager::Get()->GetLogManager()->DebugLog(_T("Test: %s <-> %s"), wconfig.c_str(), pconfig.c_str());
                                if (wconfig.StartsWith(pconfig) || pconfig.StartsWith(wconfig))
                                    targetProj = proj._project->GetBuildTarget(l);
                            }
                        }
                    }
                    else // msvc7
                    {
                        configIt = proj._configurations.find(_workspaceConfigurations[j]);
                        if (configIt != proj._configurations.end())
                            targetProj = proj._project->GetBuildTarget(configIt->second);
                    }

                    if (dep._configurations.empty()) // msvc6
                    {
                        wconfig = _workspaceConfigurations[j];
                        targetDep = dep._project->GetBuildTarget(wconfig);
                        if (targetDep == 0)
                        {
                            // look for a project config which is a substring of the workspace config
                            for (int l=0; l<dep._project->GetBuildTargetsCount(); ++l)
                            {
                                pconfig = dep._project->GetBuildTarget(l)->GetTitle();
                                //Manager::Get()->GetLogManager()->DebugLog(_T("Test: %s <-> %s"), wconfig.c_str(), pconfig.c_str());
                                if (wconfig.StartsWith(pconfig) || pconfig.StartsWith(wconfig))
                                    targetDep = dep._project->GetBuildTarget(l);
                            }
                        }
                    }
                    else // msvc7
                    {
                        configIt = dep._configurations.find(_workspaceConfigurations[j]);
                        if (configIt != dep._configurations.end())
                            targetDep = dep._project->GetBuildTarget(configIt->second);
                    }

                    if ((targetDep==0) || (targetProj==0))
                    {
                        Manager::Get()->GetLogManager()->DebugLog(_T("ERROR: could not find targets"));
                        continue;
                    }

                    Manager::Get()->GetLogManager()->DebugLog(F(_T("Match '%s' to '%s'"), targetProj->GetFullTitle().wx_str(), targetDep->GetFullTitle().wx_str()));

                    // now, update dependencies
                    TargetType type = targetDep->GetTargetType();
                    wxFileName fname;
                    if (type==ttDynamicLib)
                    {
                        // targetDep->GetStaticLibFilename() do not work since it uses the filename instead of output filename
                        Compiler* compiler = CompilerFactory::GetCompiler(depIt->second._project->GetCompilerID());
                        if (compiler)
                        {
                            wxString prefix = compiler->GetSwitches().libPrefix;
                            wxString suffix = compiler->GetSwitches().libExtension;
                            fname = targetDep->GetOutputFilename();
                            fname.SetName(prefix + fname.GetName());
                            fname.SetExt(suffix);
                        }
                    }
                    else if (type==ttStaticLib) fname = targetDep->GetOutputFilename();
                    targetProj->AddLinkLib(fname.GetFullPath());
                    targetProj->AddTargetDep(targetDep);
                    // TO REMOVE
                    wxString deps = targetProj->GetExternalDeps();
                    deps << fname.GetFullPath() << _T(';');
                    targetProj->SetExternalDeps(deps);
                    // ---------
               }
            }
            else
                Manager::Get()->GetLogManager()->DebugLog(_T("ERROR: dependency not found ") + proj._dependencyList[i]);
        }
    }
}
