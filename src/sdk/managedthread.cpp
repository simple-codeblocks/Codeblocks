/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 9426 $
 * $Id: managedthread.cpp 9426 2013-11-02 19:42:20Z alpha0010 $
 * $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/sdk/managedthread.cpp $
 */

#include "sdk_precomp.h"
#include "managedthread.h"

#include <wx/utils.h>
/// Keeps count of running threads

wxCriticalSection ManagedThread::s_count_running_mutex;
wxCriticalSection ManagedThread::s_list_mutex;
unsigned long ManagedThread::s_running   = 0;
bool          ManagedThread::s_abort_all = false;

static ManagedThreadsArray s_threadslist;

ManagedThread::ManagedThread(bool* abortflag) :
wxThread(wxTHREAD_JOINABLE),
m_pAbort(abortflag)
{
    wxCriticalSectionLocker lock(s_list_mutex);
    s_threadslist.Add(this);
}

ManagedThread::~ManagedThread()
{
    // NOTE: DeleteThreadFromList() locks the mutex by itself
    DeleteThreadFromList(this); // Deletes thread from list
}

unsigned long ManagedThread::count_running()
{
    wxCriticalSectionLocker lock(ManagedThread::s_count_running_mutex);
    return ManagedThread::s_running;
}

unsigned long ManagedThread::count_threads()
{
    wxCriticalSectionLocker lock(ManagedThread::s_list_mutex);
    return s_threadslist.GetCount();
}

ManagedThread* ManagedThread::GetThread(size_t n)
{
    wxCriticalSectionLocker lock(ManagedThread::s_list_mutex);
    if (n>=s_threadslist.GetCount())
        return nullptr;
    return s_threadslist.Item(n);
}


void ManagedThread::abort_all()
{
    // 1) Send signal to threads telling to terminate ASAP
    if (count_running() > 0)
    {
        ManagedThread::s_abort_all = true;
        while (count_running() > 0)
            wxMilliSleep(5);

        wxMilliSleep(50);

        // (there's a tiny delay between the thread disminishing the count
        // and the thread actually stopping)
        // 50 ms should be more than enough
    }

    // 2) Delete thread objects
    ManagedThread *thread;
    for (unsigned int i = 0; i < count_threads();++i)
    {
        thread = GetThread(i);
        if (thread)
        {
            if (thread->IsAlive())
                thread->Delete();
            delete thread;
        }
    }

    // 3) Clear thread list
    wxCriticalSectionLocker lock(ManagedThread::s_list_mutex);
    s_threadslist.Clear();

    // 4) Reset the abort flag now that no threads are running
    ManagedThread::s_abort_all = false;
}

void ManagedThread::abort(bool* flag,bool delete_thread)
{
    if (!flag)
        return;

    // 1) Send signal to threads telling to terminate ASAP
    if (count_running() > 0)
    {
        *flag = true;
        wxMilliSleep(50);
    }

    // 2) Delete thread objects
    ManagedThread* thread;
    for (unsigned int i = 0; i < count_threads();++i)
    {
        thread = GetThread(i);
        if (!thread)
            continue;
        if (thread->get_abort_location()!=flag)
            continue;
        if (thread->IsAlive())
            thread->Delete();
        if (delete_thread)
            delete thread;
    }

    // 4) Reset the abort flag now that no associated threads are running
    *flag = false;
}

void* ManagedThread::Entry()
{
    {
        wxCriticalSectionLocker lock(s_count_running_mutex);
        s_running++;
    }

    void* result = DoRun();

    {
        wxCriticalSectionLocker lock(s_count_running_mutex);
        if (s_running > 0)
            s_running--;
    }

    return result;

}

void* ManagedThread::DoRun()
{
    return nullptr;
}

void ManagedThread::DeleteThreadFromList(ManagedThread* thread)
{
    wxCriticalSectionLocker lock(ManagedThread::s_list_mutex);
    unsigned int i = 0;
    while (i < s_threadslist.GetCount())
    {
        if (s_threadslist[i]==thread)
            s_threadslist.RemoveAt(i,1);
        else
            ++i;
    }
}

bool ManagedThread::TestDestroy()
{
    return is_aborted() || wxThread::TestDestroy();
}

bool ManagedThread::is_aborted()
{
    if (ManagedThread::s_abort_all)
        return true;
    if (m_pAbort)
        return (*m_pAbort);

    return false;
}
