/* ==============================================================================
    HEADER: cmdline_defs.h		[wlcov]
    Purpose: Definitions for importing arguments from the command line
    Language: C
    Use: '#include "...."
    Major revisions:
 ==============================================================================*/
//        1          2          3          4          5          6          7

//
// lines where there is a "//B socket:" string are places to include module files
//  that can be found in addons/addons_include folder
//

/*
If you need to add a parameter, the pattern is a line like the following:
 
 "parameter_name=default_value",   ";Parameter comment/or brief explanation", ":parameter_shortname",

Instert a line like above inside the defv array.
 
Note: it is not necessary to have the parameter_shortname item. Then in this case the line for a parameter is:
 
 "parameter_name=default_value",   ";Parameter comment/or brief explanation",

After adding line for the new parameter you need to add corresponding line(s) in four routines in startrun.c:
 
 - ReadParametersCmdline
 - startrun_ParamStat
 - ReadParameterFile
 - PrintParameterFile

and if necessary in
 - CheckParameters
 
 */


#ifndef _cmdline_defs_h
#define _cmdline_defs_h

#define HEAD1	"LSST/S3"
#define HEAD2	"wlcov code for computing covariance matrix (2,3)pcf"
#define HEAD3	"..."

string defv[] = {  ";"HEAD1": " HEAD2 "\n\t " HEAD3,
    "paramfile=",			            ";Parameter input file. Overwrite what follows",

    "r=0.116355",                                     ";r",
    "theta1=0.1",                                        ";theta1 parameter",
    "thetap1=0.1",                                        ";thetap1 parameter",
    "theta2=0.1",                                        ";theta2 parameter",
    "thetap2=0.1",                                        ";thetap2 parameter",
    "m=0",                                       ";m parameter",
    "mp=0",                                       ";mp parameter",
    "ppp=20",                                       ";ppp parameter",

    //B Parameters to control the I/O file(s)
    // Input catalog parameters
    "inputfile=CkappaT_ep.dat",   ";input file", ":in",
    // Output parameters
    "rootDir=Output",                               ";Output dir, where output files will be written", ":root",
    //E

    "ellmax=2000.0",                               ";ellmax",
    "ellmin=1",                                 ";ellmin",

    //B Miscellaneous parameters
    "verbose=2",                        ";Option to activate the amount of information sent to standard output", ":verb",
    "verbose_log=1",                    ";Option to activate the amount of information sent to log file", ":verblog",
#ifdef OPENMPCODE
    "numberThreads=16",                  ";To set the number of threads to use (OpenMP)", ":nthreads",
#endif
    "options=",                         ";Various control options, i.e.,... , etc.", ":opt",
    //E

//B socket:
#ifdef ADDONS
#include "cmdline_defs_include.h"
#endif
//E

    "Version=1.0.0",			        ";S. Samario-Nava & A. Aviles (2026--)",
    NULL,
};

#endif // ! _cmdline_defs_h
