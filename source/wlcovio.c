/*==============================================================================
 MODULE: cballsio.c		[wlcov]
 Written by: Mario A. Rodriguez-Meza
 Starting date:	01.05.2026
 Purpose: Routines to drive input and output data
 Language: C
 Use:
 Major revisions:
 ==============================================================================*/
//        1          2          3          4        ^ 5          6          7

//
// lines where there is a "//B socket:" string are places to include module files
//  that can be found in addons/addons_include folder
//

#include "globaldefs.h"


/*
 EndRun routine:

 To be called in main:
    EndRun(&cmd, &gd);

 This routine is in charge of closing log file, printing a summary
    of the run and freeing the allocated memory

 Arguments:
    * `cmd`: Input: structure cmdline_data pointer
    * `gd`: Input: structure global_data pointer
 Return (the error status):
    int SUCCESS or FAILURE
 */
int EndRun(struct cmdline_data* cmd, struct  global_data* gd)
{
    string routineName = "EndRun";
    stream outstr;

    debug_tracking_s("001", routineName);

    if (cmd->verbose_log>0)
        fclose(gd->outlog);

    if (cmd->verbose > VERBOSENOINFO) {
        real cpuTotal = CPUTIME - gd->cpuinit;
        printf("\nFinal CPU time : %lf %s\n",
               cpuTotal, PRNUNITOFTIMEUSED);
        printf("Final real time: %ld",
               (rcpu_time()-gd->cpurealinit));
        printf(" %s\n\n", PRNUNITOFTIMEUSED);       // Only work this way
    }

    debug_tracking("002");

    EndRun_FreeMemory(cmd, gd);

    debug_tracking_s("003... final", routineName);

    return SUCCESS;
}

//
// We must check the order of memory allocation and deallocation!!!
//
int EndRun_FreeMemory(struct cmdline_data* cmd,
                             struct  global_data* gd)
{
    string routineName = "EndRun_FreeMemory";

    debug_tracking_s("001", routineName);

    if (gd->histograms_allocated == TRUE)
        EndRun_FreeMemory_histograms(cmd, gd);

    if (gd->gd_allocated == TRUE)
        EndRun_FreeMemory_gd(cmd, gd);

    if (gd->cmd_allocated == TRUE)
        EndRun_FreeMemory_cmd(cmd, gd);

    debug_tracking_s("003... final", routineName);

    return SUCCESS;
}

int EndRun_FreeMemory_histograms(struct cmdline_data* cmd,
                             struct  global_data* gd)
{
//    free_dvector(gd->histN2pcf,1,cmd->sizeHistN);

    gd->histograms_allocated = FALSE;

    return SUCCESS;
}

int EndRun_FreeMemory_gd(struct cmdline_data* cmd,
                             struct  global_data* gd)
{
    string routineName = "EndRun_FreeMemory_gd";

    debug_tracking_s("001", routineName);

    gd->gd_allocated = FALSE;

    return SUCCESS;
}

int EndRun_FreeMemory_cmd(struct cmdline_data* cmd,
                             struct  global_data* gd)
{
    string routineName = "EndRun_FreeMemory_cmd";
    debug_tracking_s("001", routineName);

    if (cmd->options!=NULL)
        free(cmd->options);
    if (gd->rootDirFlagFree==TRUE)
        free(cmd->rootDir);

    gd->cmd_allocated = FALSE;

    return SUCCESS;
}

