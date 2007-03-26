/* copyright Patrick Kooman, 2002

  Lightweight C & C++ profiler. Works both in
  debug- and release mode. For more info, read:

  http://www.2dgame-tutorial.com/sdl/profile.htm

  You are free to use / modify / re-distribute this code.

  */
#ifndef _PROFILE_H_
#define _PROFILE_H_

#include "FreeImageAlgorithms.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#ifdef DONT_PROFILE
/* Profiling disabled: compiler won't generate machine instructions now. */
#define PROFILE_START(t)    (void*)0
#define PROFILE_STOP(t)     (void*)0
#define PROFILE_PRINT()     (void*)0
#define PROFILE_RESET()     (void*)0
#else
/* Profiling enabled */
#define MAX_TAG_LEN       100
#define NUM_TAGS          100

typedef struct {
  char str_name [MAX_TAG_LEN] ;
  int i_calls ;
  clock_t start_time ;
  int i_stopped ;
  long l_total_ms ;
} entry_t ;

/* Compiler calls functions now. */
#define PROFILE_START(t)    ProfileStart (t)
#define PROFILE_STOP(t)     ProfileStop (t)
#define PROFILE_PRINT()     ProfilePrint ()
#define PROFILE_RESET()     ProfileReset ()

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Start timer for given tag */
DLL_API void DLL_CALLCONV ProfileStart (char* str_tag) ;
/* Stops timer for given tag and add time to total time for this tag */
DLL_API void DLL_CALLCONV ProfileStop (char* str_tag) ;
/* Prints result to stdout */
DLL_API void DLL_CALLCONV ProfilePrint (void) ;
/* Resets the profiler. */
DLL_API void DLL_CALLCONV ProfileReset (void) ;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NO_PROFILE */

#endif /* _PROFILE_H_ */
