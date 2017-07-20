#ifndef _DTRACE_H
#define _DTRACE_H

#include <iostream>
//**************************************************************
//useful macros  for debugging
//**************************************************************
#ifdef TRACE_LOGGING

inline void _printDebugLine(const std::string& fileName, int lineNumber)
{
unsigned found = fileName.find_last_of("/\\");
std::cerr  << "["<< lineNumber  << "]: ---"<< fileName.substr(found+1)<< " ---"<<std::endl;
}

#define dtrace(...)                         \
    (_printDebugLine(__FILE__, __LINE__),   \
    fprintf(stderr, __VA_ARGS__),           \
    (void) fprintf(stderr, "\n"))

#define derror(...)                         \
    (_printDebugLine(__FILE__, __LINE__),   \
    fprintf(stderr, "ERROR: "),             \
    fprintf(stderr, __VA_ARGS__)            \
    )
#else

#define dtrace(...)    ((void) 0)
#define derror(...)                         \
    (fprintf(stderr, __VA_ARGS__)           \
    )
#endif /* TRACE_LOGGING */


#endif 
