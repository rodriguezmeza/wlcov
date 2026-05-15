/*==============================================================================
 HEADER: globaldefs.h        [wlcov]
 Purpose: Definitions of global variables and parameters
 Language: C
 Use: '#include "globaldefs.h"
 Major revisions:
 ==============================================================================*/
//        1          2          3          4        ^ 5          6          7
#ifndef _globaldefs_h
#define _globaldefs_h

#include <stdio.h>
#include <gsl/gsl_sf_bessel.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <stddef.h>

#include "common_defs.h"

//B socket:
#ifdef ADDONS
#include "globaldefs_include_01.h"
#endif
//E

#if !defined(global)
#  define global extern
#endif

typedef char *string;

#include "cmdline_data.h"
#include "global_data.h"


//B CLASSLIB section
// standard libraries from Julien Lesgourges CLASS
#ifdef CLASSLIB
#include "common.h"
global ErrorMsg errmsg;
#endif // ! CLASSLIB
//E

//B socket:
#ifdef ADDONS
#include "globaldefs_include_02.h"
#endif
//E

#include "protodefs.h"

#endif  // ! _globaldefs_h
