/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef CBTHREADPOOL_H
#define CBTHREADPOOL_H

#include <wx/thread.h>
#include <wx/event.h>
#include <vector>
#include <list>

#include "cbthreadedtask.h"
#include "settings.h"
#include "prep.h"

/// A Thread Pool implementation
class DLLIMPORT cbThreadPool
{
  public:
    /** cbThreadPool ctor
      *
      * @param owner Event handler to receive cbEVT_THREADTASK_ENDED and cbEVT_THREADTASK_ALLDONE events
      * @param id Used with the events
      * @param concurrentThreads Number of threads in the pool. -1 means current CPU count
      */
    cbThreadPool(wxEvtHandler *owner, int id = -1, int concurrentThreads = -1, unsigned int stackSize = 0);

    /// cbThreadPool dtor
    ~cbThreadPool();

    /** Changes the number of threads in the pool
      *
      * @param concurrentThreads New number of threads. -1 means current CPU count
      * @note If tasks are running, it'll delay it until they're all done.
      */
    void SetConcurrentThreads(int concurrentThreads);

    /** Gets the current number of threads in the pool
      *
      * @return Number of threads in the pool
      * @note If a call to SetConcurrentThreads hasn't been applied, it'll return the
      * number of threads that will be set by it when all tasks be done.
      */
    int GetConcurrentThreads() const;

    /** return the pool ID */
    int GetId() const { return m_ID; }

    /** Adds a new task to the pool
      *
      * @param task The task to execute
      * @param autodelete If true, the task will be deleted when it finish or be aborted
      */
    void AddTask(cbThreadedTask *task, bool autodelete = true);

    /** Aborts all running and pending tasks
      *
      * @note Calls cbThreadedTask::Abort for all running tasks and just removes the pending ones.
      */
    void AbortAllTasks();

    /** Tells if the pool has finished its job
      *
      * @return true if it has nothing to do, false if it's executing tasks
      */
    bool Done() const;

    /** Begin a batch process
      *
      * @note EVIL: Call it if you want to add all tasks first and get none executed yet.
      * If you DON'T call it, taks will be executed as you add them (in fact it's what
      * one would expect).
      */
    void BatchBegin();

    /** End a batch process
      *
      * @note EVIL: Call it when you have finished adding tasks and want them to execute.
      * BEWARE: if you call BatchBegin but DON'T call BatchEnd, the tasks WON'T execute.
      */
    void BatchEnd();

  private:

    /// Josuttis' implementation of CountedPtr
    template <typename T>
    class CountedPtr
    {
      private:
        T *ptr;
        long *count;

      public:
        explicit CountedPtr(T *p = 0);
        CountedPtr(const CountedPtr<T> &p) throw();
        ~CountedPtr() throw();
        CountedPtr<T> &operator = (const CountedPtr<T> &p) throw();
        T &operator * () const throw();
        T *operator -> () const throw();

      private:
        void dispose();
    };

    /** A Worker Thread class.
      *
      * These are the ones that execute the tasks.
      * You shouldn't worry about it since it's for "private" purposes of the Pool.
      */
    class cbWorkerThread : public wxThread
    {
      public:
        /** cbWorkerThread ctor
          *
          * @param pool Thread Pool this Worker Thread belongs to
          * @param semaphore Used to synchronise the Worker Threads
          */
        cbWorkerThread(cbThreadPool *pool, CountedPtr<wxSemaphore> &semaphore);

        /// Entry point of this thread. The magic happens here.
        ExitCode Entry();

        /// Tell the thread to abort. It will also tell the task to abort (if any)
        void Abort();

        /** Tells whether we should abort or not
          *
          * @return true if we should abort
          */
        bool Aborted() const;

        /// Aborts the running task (if any)
        void AbortTask();

      private:
        bool m_abort;
        cbThreadPool *m_pPool;
        CountedPtr<wxSemaphore> m_semaphore;
        cbThreadedTask *m_pTask;
        wxMutex m_taskMutex;
    };

    typedef std::vector<cbWorkerThread *> WorkerThreadsArray;

    /// All tasks are added to one of these. It'll also save the autodelete value
    struct cbThreadedTaskElement
    {
      cbThreadedTaskElement(cbThreadedTask *_task = nullptr, bool _autodelete = false)
      : task(_task),
        autodelete(_autodelete)
      {
        // empty
      }

      /// It'll delete the task only if it was set to
      void Delete()
      {
        if (autodelete)
        {
          delete task;
          task = nullptr; // better safe than sorry
        }
      }

      cbThreadedTask *task;
      bool autodelete;
    };

    typedef std::list<cbThreadedTaskElement> TasksQueue;

    wxEvtHandler *m_pOwner;
    int m_ID;
    bool m_batching;

    int m_concurrentThreads; // current number of concurrent threads
    unsigned int m_stackSize; // stack size for every threads
    int m_concurrentThreadsSchedule; // if we cannot apply the new value of concurrent threads, keep it here
    WorkerThreadsArray m_threads; // the working threads are stored here
    TasksQueue m_tasksQueue; // and the pending tasks here
    bool m_taskAdded; // true if any task added

    int m_workingThreads; // how many working threads are running a task

    mutable wxMutex m_Mutex; // we better be safe

    CountedPtr<wxSemaphore> m_semaphore; // used to synchronise the Worker Threads

    void _SetConcurrentThreads(int concurrentThreads); // like SetConcurrentThreads, but non-thread safe
    void Broadcast(); // awakes all threads
    void AwakeNeeded(); // awakes only a few threads

  protected:
    friend class cbWorkerThread;

    /** Returns the next task to run
      *
      * @return Next task to run, or a NULL task (set in .task) if none
      */
    cbThreadedTaskElement GetNextTask();

    /// Mechanism for the threads to tell the Pool they're running
    void WorkingThread();

    /** Mechanism for the threads to tell the Pool they're done and will wait
      *
      * @return true if everything is OK, false if we should abort
      */
    bool WaitingThread();

    /** Called by a Worker Thread to inform a task has finished
      *
      * @param thread The Worker Thread
      */
    void TaskDone(cbWorkerThread *thread);
};

/* ************************************************ */
/* **************** INLINE MEMBERS **************** */
/* ************************************************ */

inline cbThreadPool::cbThreadPool(wxEvtHandler *owner, int id, int concurrentThreads, unsigned int stackSize)
: m_pOwner(owner),
  m_ID(id),
  m_batching(false),
  m_concurrentThreads(-1),
  m_stackSize(stackSize),
  m_concurrentThreadsSchedule(0),
  m_taskAdded(false),
  m_workingThreads(0),
  m_semaphore(new wxSemaphore)
{
  SetConcurrentThreads(concurrentThreads);
}

inline int cbThreadPool::GetConcurrentThreads() const
{
  wxMutexLocker lock(m_Mutex);
  return m_concurrentThreadsSchedule ? m_concurrentThreadsSchedule : m_concurrentThreads;
}

inline bool cbThreadPool::Done() const
{
  wxMutexLocker lock(m_Mutex);
  return m_workingThreads == 0;
}

inline void cbThreadPool::BatchBegin()
{
  wxMutexLocker lock(m_Mutex);
  m_batching = true;
}

inline void cbThreadPool::Broadcast()
{
  if (m_concurrentThreads == -1)
    return;

  for (std::size_t i = 0; i < static_cast<std::size_t>(m_concurrentThreads - m_workingThreads); ++i)
    m_semaphore->Post();
}

inline void cbThreadPool::AwakeNeeded()
{
  if (m_concurrentThreads == -1)
    return;

  for (std::size_t i = 0; i < m_tasksQueue.size(); ++i)
    m_semaphore->Post();
}

/* *** Josuttis' CountedPtr *** */

template <typename T>
inline cbThreadPool::CountedPtr<T>::CountedPtr(T *p)
: ptr(p),
  count(new long(1))
{
  // empty
}

template <typename T>
inline cbThreadPool::CountedPtr<T>::CountedPtr(const CountedPtr<T> &p) throw()
: ptr(p.ptr),
  count(p.count)
{
  ++*count;
}

template <typename T>
inline cbThreadPool::CountedPtr<T>::~CountedPtr() throw()
{
  dispose();
}

template <typename T>
inline cbThreadPool::CountedPtr<T> &cbThreadPool::CountedPtr<T>::operator = (const CountedPtr<T> &p) throw()
{
  if (this != &p)
  {
    dispose();
    ptr = p.ptr;
    count = p.count;
    ++*count;
  }

  return *this;
}

template <typename T>
inline T &cbThreadPool::CountedPtr<T>::operator * () const throw()
{
  return *ptr;
}

template <typename T>
inline T *cbThreadPool::CountedPtr<T>::operator -> () const throw()
{
  return ptr;
}

template <typename T>
inline void cbThreadPool::CountedPtr<T>::dispose()
{
  if (--*count == 0)
  {
    delete count;
    delete ptr;
  }
}

#endif  //CBTHREADPOOL_H
