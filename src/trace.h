/* trace.h */

#ifndef TRACE_H
#define TRACE_H

#if 0
#define TRACE_ENABLE 1
#endif
#if 0
#define TRACE2_ENABLE 1
#endif

#include <stdio.h>

#ifdef TRACE_ENABLE
#define TRACE(str) fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, str)
//#define TRACE(str) printf("%s:%d: %s\n", __FILE__, __LINE__, str)
#else
#define TRACE(str) {}
#endif

#ifdef TRACE2_ENABLE
#define TRACE2(str) fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, str)
//#define TRACE2(str) printf("%s:%d: %s\n", __FILE__, __LINE__, str)
#else
#define TRACE2(str) {}
#endif

#endif
