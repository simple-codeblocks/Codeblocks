#ifndef __WX_DEBUGGING_H__
#define __WX_DEBUGGING_H__
// RCS-ID:      $Id: debugging.h 8323 2012-09-01 12:10:45Z jenslody $

//#include <manager.h>
//#include <logmanager.h>

#if defined(kbLOGGING)
    #define LOGGING 1
#endif

#if wxCHECK_VERSION(2, 9, 0)
// avoid redefinition warning
#undef wxLogMessage
#endif
#define wxLogMessage wxLogDebug
#define LOGIT wxLogDebug

#if LOGGING
    #undef wxLogMessage
    #undef LOGIT
   // wxMSW wont write msg to our log window via wxLogDebug
   #ifdef __WXMSW__
    #define TRAP asm("int3")
    #define wxLogDebug wxLogMessage
    #define LOGIT wxLogMessage
   #endif
   // wxGTK wxLogMessage turns into a wxMessage in GTK
   #ifdef __WXGTK__
    #define TRAP asm("int3")
    #define LOGIT wxLogMessage
   #endif
   #ifdef __WXMAC__
    #warning kbLOGGING and __WXMAC__  defined for debugging.h
    #define TRAP asm("trap")
    #define LOGIT wxLogMessage
    #define wxLogDebug wxLogMessage
   #endif
#endif

#endif  //__WX_DEBUGGING_H__
