/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 9737 $
 * $Id: pipedprocess.cpp 9737 2014-03-12 20:30:56Z fuscated $
 * $HeadURL: svn://svn.code.sf.net/p/codeblocks/code/trunk/src/sdk/pipedprocess.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/app.h>         // wxWakeUpIdle
    #include "pipedprocess.h" // class' header file
    #include "sdk_events.h"
    #include "globals.h"
#endif

// The following class is created to override wxTextStream::ReadLine()
class cbTextInputStream : public wxTextInputStream
{
    protected:
        bool m_allowMBconversion;
    public:
#if wxUSE_UNICODE
        cbTextInputStream(wxInputStream& s, const wxString &sep=wxT(" \t"), wxMBConv& conv = wxConvLocal )
            : wxTextInputStream(s, sep, conv),
            m_allowMBconversion(true)
        {
            memset((void*)m_lastBytes, 0, 10);
        }
#else
        cbTextInputStream(wxInputStream& s, const wxString &sep=wxT(" \t") )
            : wxTextInputStream(s, sep)
        {
            memset((void*)m_lastBytes, 0, 10);
        }
#endif
        ~cbTextInputStream(){}


        // The following function was copied verbatim from wxTextStream::NextChar()
        // The only change, is the removal of the MB2WC function
        // With PipedProcess we work with compilers/debuggers which (usually) don't
        // send us unicode (at least GDB).
        wxChar NextChar()
        {
        #if wxUSE_UNICODE
            wxChar wbuf[2];
            memset((void*)m_lastBytes, 0, 10);
            for (size_t inlen = 0; inlen < 9; inlen++)
            {
                // actually read the next character
                m_lastBytes[inlen] = m_input.GetC();

                if (m_input.LastRead() <= 0)
                    return wxEOT;
                if (m_allowMBconversion)
                {
                    int retlen = (int) m_conv->MB2WC(wbuf, m_lastBytes, 2); // returns -1 for failure
                    if (retlen >= 0) // res == 0 could happen for '\0' char
                        return wbuf[0];
                }
                else
                    return m_lastBytes[inlen]; // C::B fix (?)
            }
            // there should be no encoding which requires more than nine bytes for one character...
            return wxEOT;
        #else
            m_lastBytes[0] = m_input.GetC();

            if (m_input.LastRead() <= 0)
                return wxEOT;

            return m_lastBytes[0];
        #endif
        }

        // The following function was copied verbatim from wxTextStream::ReadLine()
        // The only change, is the addition of m_input.CanRead() in the while()
        wxString ReadLine()
        {
            wxString line;

            while ( m_input.CanRead() && !m_input.Eof() )
            {
                wxChar c = NextChar();
                if (m_input.LastRead() <= 0)
                    break;

                if ( !m_input )
                    break;

                if (EatEOL(c))
                    break;

                line += c;
            }
            return line;
        }
};

int idTimerPollProcess = wxNewId();

BEGIN_EVENT_TABLE(PipedProcess, wxProcess)
    EVT_TIMER(idTimerPollProcess, PipedProcess::OnTimer)
    EVT_IDLE(PipedProcess::OnIdle)
END_EVENT_TABLE()

// class constructor
PipedProcess::PipedProcess(PipedProcess** pvThis, wxEvtHandler* parent, int id, bool pipe, const wxString& dir)
    : wxProcess(parent, id),
    m_Parent(parent),
    m_Id(id),
    m_Pid(0),
    m_pvThis(pvThis)
{
    wxSetWorkingDirectory(UnixFilename(dir));
    if (pipe)
        Redirect();
}

// class destructor
PipedProcess::~PipedProcess()
{
    // insert your code here
}

int PipedProcess::Launch(const wxString& cmd, cb_unused unsigned int pollingInterval)
{
    m_Pid = wxExecute(cmd, wxEXEC_ASYNC, this);
    if (m_Pid)
    {
//        m_timerPollProcess.SetOwner(this, idTimerPollProcess);
//        m_timerPollProcess.Start(pollingInterval);
    }
    return m_Pid;
}

void PipedProcess::SendString(const wxString& text)
{
    //Manager::Get()->GetLogManager()->Log(m_PageIndex, cmd);
    wxOutputStream* pOut = GetOutputStream();
    if (pOut)
    {
        wxTextOutputStream sin(*pOut);
        wxString msg = text + _T('\n');
        sin.WriteString(msg);
    }
}

void PipedProcess::ForfeitStreams()
{
    char buf[4096];
    if ( IsErrorAvailable() )
    {
        wxInputStream *in = GetErrorStream();
        while (in->Read(&buf, sizeof(buf)).LastRead())
            ;
    }
    if ( IsInputAvailable() )
    {
        wxInputStream *in = GetInputStream();
        while (in->Read(&buf, sizeof(buf)).LastRead())
            ;
    }
}

bool PipedProcess::HasInput()
{
    if (IsErrorAvailable())
    {
        cbTextInputStream serr(*GetErrorStream());

        wxString msg;
        msg << serr.ReadLine();

        CodeBlocksEvent event(cbEVT_PIPEDPROCESS_STDERR, m_Id);
        event.SetString(msg);
        wxPostEvent(m_Parent, event);
//         m_Parent->ProcessEvent(event);

        return true;
    }

    if (IsInputAvailable())
    {
        cbTextInputStream sout(*GetInputStream());

        wxString msg;
        msg << sout.ReadLine();

        CodeBlocksEvent event(cbEVT_PIPEDPROCESS_STDOUT, m_Id);
        event.SetString(msg);
        wxPostEvent(m_Parent, event);
//         m_Parent->ProcessEvent(event);

        return true;
    }

    return false;
}

void PipedProcess::OnTerminate(int /*pid*/, int status)
{
    // show the rest of the output
    while ( HasInput() )
        ;

    CodeBlocksEvent event(cbEVT_PIPEDPROCESS_TERMINATED, m_Id);
    event.SetInt(status);
//       m_Parent->ProcessEvent(event);
    wxPostEvent(m_Parent, event);

    if (m_pvThis)
        *m_pvThis = nullptr;
    delete this;
}

void PipedProcess::OnTimer(cb_unused wxTimerEvent& event)
{
    wxWakeUpIdle();
}

void PipedProcess::OnIdle(cb_unused wxIdleEvent& event)
{
    while ( HasInput() )
        ;
}
