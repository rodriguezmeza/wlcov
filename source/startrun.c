/*==============================================================================
 MODULE: startrun.c				[wlcf]
 Written by: Mario A. Rodriguez-Meza
 Starting date: 01.05.2026
 Purpose: routines to initialize the main code
 Language: C
 Use: 'StartRun();'
 
 Mayor revisions:
 ==============================================================================*/
//        1          2          3          4        ^ 5          6          7

//
// We must check the order of memory allocation and deallocation!!!
// Here and in EndRun in cballsio.c
//

//
// lines where there is a "//B socket:" string are places to include module files
//  that can be found in addons/addons_include folder
//

#include "globaldefs.h"

local void ReadParameterFile(struct  cmdline_data*, struct  global_data*, char *);
local int startrun_parameterfile(struct  cmdline_data*, struct  global_data*);
local int startrun_cmdline(struct  cmdline_data*, struct  global_data*);
local void ReadParametersCmdline(struct  cmdline_data*, struct  global_data*);
local void ReadParametersCmdline_short(struct  cmdline_data*, 
                                       struct  global_data*);
local int CheckParameters(struct  cmdline_data*, struct  global_data*);

//B Special routines to scan command line
local int startrun_getParamsSpecial(struct  cmdline_data*, struct  global_data*);
local int scaniOption(struct  cmdline_data*, struct  global_data*,
                      string, int *, int *, int, int, string);
local int scanrOption(struct  cmdline_data*, struct  global_data*,
                      string, double *, int *, int, int, string);
//E
//B I/O directories:
local void setFilesDirs_log(struct cmdline_data*, struct  global_data* gd);
local void setFilesDirs(struct cmdline_data*, struct  global_data* gd);
//E
local int print_make_info(struct cmdline_data* cmd,
                     struct  global_data* gd);

/*
//B from IntegralCovMatrix
double ell_data[n_data_max],Cls_data[n_data_max];
int n_data;

double max_of_two(double a, double b);
double max_of_three(double a, double b, double c);

double Bessel_Jn(int n, double x);
double Cls(double ell);

double to_integrate(int m, int mp, double r,
            double theta1, double thetap1, double theta2, double thetap2,
            int ppp, double ellmin, double ellmax);


double xi_integrad( double r, double ell);
double xi( double r, int points_per_period, double ellmin, double ellmax);

double fmmp_integrand(double ell, int m, int mp, double theta, double thetap, double r);
double fmmp(int m, int mp, double r, double theta, double thetap,
            int points_per_period, double ellmin, double ellmax);

double fm_integrand(double ell, int m, double theta, double r);
double fm(int m, double r, double theta, int points_per_period, double ellmin,
        double ellmax);

double fmp_integrand(double ell, int mp, double thetap, double r);
double fmp(int mp, double r, double thetap, int points_per_period, double ellmin,
            double ellmax);

double fmmp11;
double fmmp22;
double fmmp12;
double fmmp21;
double fm_nmp12;
double fm_nmp21;
double fm1;
double fm2;
double fmp1;
double fmp2;
double integrand;
double xiplus;
double r_int;
*/
//E

//B socket:
#ifdef ADDONS
#include "startrun_include_00.h"
#endif
//E

/*
 StartRun routine:

 To be called in main:
 StartRun(&cmd, &gd, argv[0], HEAD1, HEAD2, HEAD3);
 
 This routine is in charge of setting all global structures in order to
    the comutation process run smoothly with all parameters given
    by the user, set and checked.

 Arguments:
    * `cmd`: Input: structure cmdline_data pointer
    * `gd`: Input: structure global_data pointer
    * `head0`: Input: string
    * `head1`: Input: string
    * `head2`: Input: string
    * `head3`: Input: string
 Return (the error status):
    int SUCCESS or FAILURE
 */
#ifndef CLASSLIB
int StartRun(struct  cmdline_data* cmd, struct  global_data* gd, 
             string head0, string head1, string head2, string head3)
{
    string routineName = "StartRun";
    double cpustart = CPUTIME;

    gd->headline0 = head0; gd->headline1 = head1;
    gd->headline2 = head2; gd->headline3 = head3;

    printf("\n%s\n%s: %s\n\t %s\n",
           gd->headline0, gd->headline1, gd->headline2, gd->headline3);
    printf("Version = %s\n", getversion());

    //B move all these to Startrun_Common... or make an appropriate change
    gd->cmd_allocated = FALSE;
    gd->cputotalinout = 0.;
    gd->cputotal = 0.;
    gd->bytes_tot = 0;
    //E

    cmd->paramfile = GetParam("paramfile");
    if (*(cmd->paramfile)=='-')
        error("bad parameter %s\n", cmd->paramfile);
    if (!strnull(cmd->paramfile))
		startrun_parameterfile(cmd, gd);
    else
		startrun_cmdline(cmd, gd);

    gd->bytes_tot += sizeof(struct  global_data);
    gd->bytes_tot += sizeof(struct cmdline_data);
    verb_print_min_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                "\n%s: Total allocated %g MByte storage so far.\n",
                        routineName, gd->bytes_tot*INMB);

//B If uncommented there will be a warning in the setup.py process
//#ifdef OPENMPCODE
    class_call_cballs(SetNumberThreads(cmd), errmsg, errmsg);
//#endif
//E
    gd->cputotalinout += CPUTIME - cpustart;
    verb_print_min_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                        "\n%s CPU time: %g %s\n",
                        routineName, CPUTIME - cpustart, PRNUNITOFTIMEUSED);

    return SUCCESS;
}

#else // ! CLASSLIB

#include "input.h"

int StartRun(struct  cmdline_data* cmd, struct  global_data* gd,
             string head0, string head1, string head2, string head3)
{
    string routineName = "StartRun";
    struct file_content fc;

    double cpustart = CPUTIME;
    
    gd->headline0 = head0; gd->headline1 = head1;
    gd->headline2 = head2; gd->headline3 = head3;
    printf("\n%s\n%s: %s\n\t %s\n",
           gd->headline0, gd->headline1, gd->headline2, gd->headline3);
    printf("Version = %s\n", getversion());

    //B move all these to Startrun_Common... or make an appropriate change
    gd->cmd_allocated = FALSE;
    gd->cputotalinout = 0.;
    gd->cputotal = 0.;
    gd->bytes_tot = 0;
    //E

    cmd->paramfile = GetParam("paramfile");
    if (*(cmd->paramfile)=='-')
        error("bad parameter %s\n", cmd->paramfile);
    if (!strnull(cmd->paramfile)) {
        class_call_cballs(input_find_file(cmd, gd, cmd->paramfile, &fc, errmsg),
                          errmsg, errmsg);
        class_call_cballs(input_read_from_file(cmd, gd, &fc, errmsg),
                          errmsg, errmsg);
        class_call_cballs(parser_free(&fc), errmsg, errmsg);
    } else {
        startrun_cmdline(cmd, gd);
    }

    if (!strnull(cmd->paramfile))
        class_call_cballs(StartRun_Common(cmd, gd), errmsg, errmsg);

        if (!strnull(cmd->paramfile))
            PrintParameterFile(cmd, gd, cmd->paramfile);

    gd->bytes_tot += sizeof(struct  global_data);
    gd->bytes_tot += sizeof(struct cmdline_data);
    verb_print(cmd->verbose,
               "\n%s: Total allocated %g MByte storage so far.\n",
               routineName, gd->bytes_tot*INMB);

#ifdef OPENMPCODE
    class_call_cballs(SetNumberThreads(cmd), errmsg, errmsg);
#endif

    gd->cputotalinout += CPUTIME - cpustart;
    verb_print(cmd->verbose, "\n%s CPU time: %g %s\n",
               routineName, CPUTIME - cpustart, PRNUNITOFTIMEUSED);

    return SUCCESS;
}
#endif // ! CLASSLIB


local int startrun_parameterfile(struct  cmdline_data* cmd,
                                 struct  global_data* gd)
{
	ReadParameterFile(cmd, gd, cmd->paramfile);
    ReadParametersCmdline_short(cmd, gd);

//B socket:
#ifdef ADDONS
#include "startrun_include_01.h"
#endif
//E

	StartRun_Common(cmd, gd);
    PrintParameterFile(cmd, gd, cmd->paramfile);

    return SUCCESS;
}


#define parameter_null	"parameters_null-wlcov"

//B Section for reading parameters from the command line

local int startrun_cmdline(struct  cmdline_data* cmd, struct  global_data* gd)
{
	ReadParametersCmdline(cmd, gd);
	StartRun_Common(cmd, gd);
//    if (gd->flagPrint==TRUE && gd->rootDirFlag==TRUE) {
        PrintParameterFile(cmd, gd, parameter_null);
//    }

    return SUCCESS;
}

local void ReadParametersCmdline(struct  cmdline_data* cmd, 
                                 struct  global_data* gd)
{
// Every item in cmdline_defs.h must have an item here::

    //B Parameters related to background cosmology
    cmd->r = GetdParam("r");
    cmd->theta1 = GetdParam("theta1");
    cmd->thetap1 = GetdParam("thetap1");
    cmd->theta2 = GetdParam("theta2");
    cmd->thetap2 = GetdParam("thetap2");
    cmd->m = GetiParam("m");
    cmd->mp = GetiParam("mp");
    cmd->ppp = GetiParam("ppp");
    //E

    //B Parameters for I/O
    cmd->rootDir = GetParam("rootDir");
    cmd->inputfile = GetParam("inputfile");
    cmd->ellmax = GetdParam("ellmax");
    cmd->ellmin = GetdParam("ellmin");
    //E

    //B Miscellaneous parameters
    cmd->verbose = GetiParam("verbose");
    cmd->verbose_log = GetiParam("verbose_log");
#ifdef OPENMPCODE
    cmd->numthreads = GetiParam("numberThreads");
#endif
    cmd->options = GetParam("options");
    //E

//B socket:
#ifdef ADDONS
#include "startrun_include_02.h"
#endif
//E
}

local void ReadParametersCmdline_short(struct  cmdline_data* cmd, struct  global_data* gd)
{
//B Here add parameters needed to be read after reading parameter file
//    cmd->ellmax = GetdParam("ellmax");
//E
}

//E

#undef parameter_null

//B Section of parameter reading from a file
local void ReadParameterFile(struct  cmdline_data* cmd, 
                             struct  global_data* gd, char *fname)
{
// Every item in cmdline_defs.h must have an item here::
#define DOUBLE 1
#define STRING 2
#define INT 3
#define LONG 6
#define BOOLEAN 4
#define MAXTAGS 300
#define MAXCHARBUF 1024

    string routineName = "ReadParameterFile";
    FILE *fd;

  int  i,j,nt;
  int  id[MAXTAGS];
  void *addr[MAXTAGS];
  char tag[MAXTAGS][50];
  int  errorFlag=0;

    int input_verbose = 2;
    verb_print(input_verbose, "\nparsing input parameters...\n");

  nt=0;

    //B Parameters related to the cosmological background
    RPName(cmd->r,"r");
    RPName(cmd->theta1,"theta1");
    RPName(cmd->thetap1,"thetap1");
    RPName(cmd->theta2,"theta2");
    RPName(cmd->thetap2,"thetap2");
    IPName(cmd->m,"m");
    IPName(cmd->mp,"mp");
    IPName(cmd->ppp,"ppp");
    //E

    //B Parameters to control the I/O file(s)
    // Input catalog parameters
    SPName(cmd->inputfile,"inputfile",MAXLENGTHOFSTRSCMD);
    // Output parameters
    SPName(cmd->rootDir,"rootDir",MAXLENGTHOFSTRSCMD);
    //E

    //B Set of parameters needed to integrate
    RPName(cmd->ellmax,"ellmax");
    RPName(cmd->ellmin,"ellmin");
    //E

    //B Miscellaneous parameters
    IPName(cmd->verbose,"verbose");
    IPName(cmd->verbose_log,"verbose_log");
#ifdef OPENMPCODE
    IPName(cmd->numthreads,"numberThreads");
#endif
    SPName(cmd->options,"options",MAXLENGTHOFSTRSCMD);
    //E

//B socket:
#ifdef ADDONS
#include "startrun_include_03.h"
#endif
//E

    size_t slen;
    char *script1;
    char *script2;
    char *script3;
    char *script4;
    char buf4[MAXCHARBUF];
    char buf5[MAXCHARBUF];

//B
#ifndef _LINE_LENGTH_MAX_
#define _LINE_LENGTH_MAX_ 1024
#endif
#define _ARGUMENT_LENGTH_MAX_ 1024
        char line[_LINE_LENGTH_MAX_];
        char name[_ARGUMENT_LENGTH_MAX_];
        char value[_ARGUMENT_LENGTH_MAX_];
        char * phash;
        char * pequal;
        char * left;
        char * right;
//E

    if((fd=fopen(fname,"r"))) {
        while(!feof(fd)) {
//B
            fgets(line,MAXCHARBUF,fd);

            pequal=strchr(line,'=');
            if (pequal == NULL)
                continue;
            phash=strchr(line,'#');
            if ((phash != NULL) && (phash-pequal<2))
                continue;
            phash=strchr(line,'%');
            if ((phash != NULL) && (phash-pequal<2))
                continue;

            left=line;
            while (left[0]==' ') {
              left++;
            }
            if(left[0]=='\'' || left[0]=='\"'){
              left++;
            }
            right=pequal-1;
            while (right[0]==' ') {
              right--;
            }
            if(right[0]=='\'' || right[0]=='\"'){
              right--;
            }

            if (right-left < 0) {
                fprintf(stdout,
        "Error in file %s: there is no variable name before '=' in line: '%s'\n",
                    fname, line);
                errorFlag=1;
                continue;
            }

            strncpy(name,left,right-left+1);
            name[right-left+1]='\0';

            left = pequal+1;
            while (left[0]==' ') {
              left++;
            }

            if (phash == NULL)
              right = line+strlen(line)-1;
            else
              right = phash-1;

            while (right[0]<=' ') {
              right--;
            }

            if (right-left < 0)
                continue;

            strncpy(value,left,right-left+1);
            value[right-left+1]='\0';
//E

            for(i=0,j=-1;i<nt;i++)
                if(strcmp(name,tag[i])==0) {
                    j=i;
                    tag[i][0]=0;
                    break;
                }
            if(j>=0) {
                switch(id[j]) {
                    case DOUBLE:
                        *((double*)addr[j])=atof(value);
                        break;
                    case STRING:
                        strcpy(addr[j],value);
                        break;
                    case INT:
                        *((int*)addr[j])=atoi(value);
                        break;
                    case LONG:
                        *((long*)addr[j])=atol(value);
                        break;
                    case BOOLEAN:
                        if (strchr("tTyY1", *value) != NULL) {
                            *((bool*)addr[j])=TRUE;
                        } else
                            if (strchr("fFnN0", *value) != NULL)  {
                                *((bool*)addr[j])=FALSE;
                            } else {
                                error("getbparam: %s=%s not bool\n",name,value);
                            }
                        break;
                }
            } else {
                fprintf(stdout, "\n%s: Error in file %s: Tag '%s' %s\n",
                        routineName, fname, name,
                        "not allowed or multiple defined...");
//                        "look at saved parameter file which value was used");
                errorFlag=1;
            }
        } // ! while loop
        fclose(fd);
    } else {
        fprintf(stdout,"Parameter file %s not found.\n", fname);
        errorFlag=2;
        exit(0);
    }

    if (errorFlag==1)
        error("%s: going out\n", routineName);

    for(i=0;i<nt;i++) {
        if(*tag[i]) {
            if (cmd->verbose>2)
                fprintf(stdout,
                    "Warning! I miss a value for tag '%s' in parameter file '%s'.\n",
                    tag[i],fname);
            switch(id[i]) {
                case DOUBLE:
                    *((double*)addr[i])=GetdParam(tag[i]);
                    break;
                case STRING:
                    strcpy(addr[i],GetParam(tag[i]));
                    break;
                case INT:
                    *((int*)addr[i])=GetiParam(tag[i]);
                    break;
                case LONG:
                    *((long*)addr[i])=GetlParam(tag[i]);
                    break;
                case BOOLEAN:
                    *((bool*)addr[i])=GetbParam(tag[i]);
                    break;
            }
            errorFlag=1;
        }
    }

#undef DOUBLE
#undef STRING
#undef INT
#undef BOOLEAN
#undef MAXTAGS
#undef MAXCHARBUF
}
//E

int StartRun_Common(struct  cmdline_data* cmd, struct  global_data* gd)
{
    string routineName = "StartRun_Common";
    double cpustart;
    double cpustartMiddle;

    debug_tracking_s("001", routineName);

    if (strlen(cmd->rootDir)==0 || strnull(cmd->rootDir))
        gd->rootDirFlag = FALSE;
    else
        gd->rootDirFlag = TRUE;

    gd->flagPrint = TRUE;

    if (scanopt(cmd->options, "make-info"))
        print_make_info(cmd, gd);

//B socket:
#ifdef ADDONS
#include "startrun_include_04.h"
#endif
//E

    debug_tracking("002");
    debug_tracking("003");
    class_call_cballs(StartOutput(cmd, gd), errmsg, errmsg);
    debug_tracking("004");
    setFilesDirs(cmd, gd);
    setFilesDirs_log(cmd, gd);
    strcpy(gd->mode,"w");
    if (cmd->verbose_log>0) {               // gd->outlog is defined
        if(!(gd->outlog=fopen(gd->logfilePath, gd->mode)))
            error("\n%s: error opening file '%s' \n",
                  routineName, gd->logfilePath);
    }

    class_call_cballs(startrun_getParamsSpecial(cmd, gd), errmsg, errmsg);
    class_call_cballs(CheckParameters(cmd, gd), errmsg, errmsg);
    class_call_cballs(startrun_memoryAllocation(cmd, gd), errmsg, errmsg);

    //B from IntegralCovMatrix
    gd->fsky = 1;                                   // should be for full-sky
                                                    //  move to cmd structure
    double rArcMin,theta1ArcMin, theta2ArcMin, thetap1ArcMin, thetap2ArcMin,radToArcMin;
    
    radToArcMin = 180.0*60.0 / M_PI;
    rArcMin       = cmd->r *radToArcMin;
    theta1ArcMin   = cmd->theta1*radToArcMin;
    theta2ArcMin   = cmd->theta2*radToArcMin;
    thetap1ArcMin  = cmd->thetap1*radToArcMin;
    thetap2ArcMin  = cmd->thetap2*radToArcMin;

    // Print the values
    printf("r= %.7f  = %.2f arcmin\n", cmd->r, rArcMin);
    printf("theta1= %.7f = %.2f arcmin\n", cmd->theta1, theta1ArcMin);
    printf("theta2= %.7f = %.2f arcmin\n", cmd->theta2, theta2ArcMin);
    printf("thetap1= %.7f = %.2f arcmin\n", cmd->thetap1, thetap1ArcMin);
    printf("thetap2= %.7f = %.2f arcmin\n", cmd->thetap2, thetap2ArcMin);
    printf("ellmin= %.3f\n", cmd->ellmin);
    printf("ellmax- %.3f\n", cmd->ellmax);
    printf("inputfile: %s\n", cmd->inputfile);
    printf("m= %d\n", cmd->m);
    printf("mp= %d\n", cmd->mp);
    printf("ppp (points_per_period)= %d\n", cmd->ppp);

    FILE *fp;
    gd->n_data=0;
    fp=fopen(cmd->inputfile,"r");
    if(fp!=NULL){
        while(fscanf(fp, "%lf %lf", &gd->ell_data[gd->n_data],
                     &gd->Cls_data[gd->n_data])!=EOF){
            gd->n_data++;
            if(gd->n_data>n_data_max) printf("n_data_max should be larger than the number of data lines \n");
        }  // ell   Ckappa
        fclose(fp);
    }
    //E

//B socket:
#ifdef ADDONS
#include "startrun_include_05.h"
#endif
//E

    debug_tracking_s("005... final", routineName);

    return SUCCESS;
}


//B Section of parameter check
local int CheckParameters(struct  cmdline_data* cmd, struct  global_data* gd)
{
// If it is necessary: an item in cmdline_defs.h must have an item here::
    string routineName = "CheckParameters";

    debug_tracking_s("001", routineName);

    if (cmd->r < 0.0)
        error("\n%s: r (%g) can not be less than 0.\n",
              routineName, cmd->r);
    if (cmd->ellmin < 0)
        error("\n%s: ellmin parameter ns (%d) can not be less than 0\n",
              routineName, cmd->ellmin);
    if (cmd->ellmax < 0)
        error("\n%s: ellmax parameter ns (%d) can not be less than 0\n",
              routineName, cmd->ellmax);

//B socket:
#ifdef ADDONS
#include "startrun_include_07.h"
#endif
//E
    debug_tracking_s("002... final", routineName);

    return SUCCESS;
}
//E


#define FMTT    "%-35s = %s\n"
#define FMTTS    "%-35s = \"%s\"\n"
#define FMTI    "%-35s = %d\n"
#define FMTIL    "%-35s = %ld\n"
#define FMTR	"%-35s = %g\n"

//B Section of parameter writing to a file
int PrintParameterFile(struct  cmdline_data *cmd,
                       struct  global_data* gd, char *fname)
{
// Every item in cmdline_defs.h must have an item here::
    string routineName = "PrintParameterFile";

    FILE *fdout;
    char buf[200];
    int  errorFlag=0;

    debug_tracking_s("001", routineName);

    if (gd->flagPrint==TRUE && gd->rootDirFlag==TRUE) {
        //B Look for "/" if fname is composed: path and filename
        int ndefault = 0;
        int ipos;
        char *dp;
        for (int i; i< strlen(fname); i++) {
            if(fname[i] == '/') {
                ipos = i+1;
                ndefault++;
            }
        }
        
        if (ndefault == 0) {
            sprintf(buf,"%s/%s%s",cmd->rootDir,fname,"-usedvalues");
        } else {
            dp = (char*) malloc((strlen(fname)-ipos)*sizeof(char));
            strncpy(dp, fname + ipos, strlen(fname)-ipos);
            verb_print_q(3,cmd->verbose,
                         "PrintParameterFile: '/' counts %d pos %d and %s\n",
                         ndefault, ipos, dp);
            sprintf(buf,"%s/%s%s",cmd->rootDir,dp,"-usedvalues");
        }
        //E
        
        if(!(fdout=fopen(buf,"w"))) {
            fprintf(stdout,"error opening file '%s' \n",buf);
            errorFlag=1;
        } else {
            fprintf(fdout,FMTR,"r",cmd->r);
            fprintf(fdout,FMTR,"theta1",cmd->theta1);
            fprintf(fdout,FMTR,"thetap1",cmd->thetap1);
            fprintf(fdout,FMTR,"theta2",cmd->theta2);
            fprintf(fdout,FMTR,"thetap2",cmd->thetap2);
            fprintf(fdout,FMTI,"m",cmd->m);
            fprintf(fdout,FMTI,"mp",cmd->mp);
            fprintf(fdout,FMTI,"ppp",cmd->ppp);

            //B Parameters to control the I/O file(s)
            // Input catalog parameters
            fprintf(fdout,FMTT,"inputfile",cmd->inputfile);
            // Output parameters
            fprintf(fdout,FMTT,"rootDir",cmd->rootDir);
            //E

            //B Set of parameters needed to integrate
            fprintf(fdout,FMTR,"ellmax",cmd->ellmax);
            fprintf(fdout,FMTR,"ellmin",cmd->ellmin);
            //E

            //B Miscellaneous parameters
            fprintf(fdout,FMTI,"verbose",cmd->verbose);
            fprintf(fdout,FMTI,"verbose_log",cmd->verbose_log);
#ifdef OPENMPCODE
            fprintf(fdout,FMTI,"numberThreads",cmd->numthreads);
#endif
            fprintf(fdout,FMTT,"options",cmd->options);
            //E
            
            //B socket:
#ifdef ADDONS
#include "startrun_include_08.h"
#endif
            //E
            
            fprintf(fdout,"\n\n");
        }
        fclose(fdout);
        
        if(errorFlag) {
            exit(0);
        }
        
        if (ndefault != 0)
            free(dp);
        
    } // ! gd->flagPrint==TRUE && gd->rootDirFlag==TRUE

    debug_tracking_s("002... final", routineName);


    return SUCCESS;
}
//E

#undef FMTT
#undef FMTTS
#undef FMTI
#undef FMTR

int startrun_memoryAllocation(struct  cmdline_data *cmd,
                                     struct  global_data* gd)
{
    string routineName = "startrun_memoryAllocation";
    // Free allocated memory in reverse order as were allocated

    INTEGER bytes_tot_local=0;
    //B PXD functions
#ifdef PXD
#endif
    bytes_tot_local += 0.0*sizeof(real);
    //E PXD functions

//B socket:
#ifdef ADDONS
    // this is empty and can be remove these 3 lines
#include "startrun_include_10.h"                    // should be sync with
                                                    //  "cballsio_include_10.h"
#endif
//E

    gd->bytes_tot += bytes_tot_local;
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "\n%s: Allocated %g MByte for histograms storage.\n",
                           routineName, bytes_tot_local*INMB);

//    gd->histograms_allocated = TRUE;

    return SUCCESS;
}

#ifdef OPENMPCODE
int SetNumberThreads(struct  cmdline_data *cmd)
{
    omp_set_num_threads(cmd->numthreads);

    return SUCCESS;
}
#endif


//B Special routines to scan command line

local int startrun_getParamsSpecial(struct  cmdline_data* cmd,
                                    struct  global_data* gd)
{
    string routineName = "startrun_getParamsSpecial";
    char *pch;
    int nitems, ndummy=1;
    char inputnametmp[MAXLENGTHOFSTRSCMD];
    int i;
    size_t slen;

    debug_tracking_s("001", routineName);


//B socket:
#ifdef ADDONS
#include "startrun_include_12.h"
#endif
//E
    debug_tracking_s("002... final", routineName);

    return SUCCESS;
}

local int scaniOption(struct  cmdline_data* cmd, struct  global_data* gd,
                      string optionstr, int *option, int *noption,
                      int nfiles, int flag, string message)
{
    string routineName = "scaniOption";
    char *pch;
    char *poptionstr[30],  optiontmp[100];
    int i;

    verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "\n%s: Processing '%s' option:\n",
                        routineName, message);

    verb_log_print(cmd->verbose_log, gd->outlog,
                           "%s: Splitting string \"%s=%s\" in tokens:\n",
                        routineName, message, optionstr);

    if (!strnull(optionstr)) {
        strcpy(optiontmp,optionstr);
        verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                               "%s: Splitting string \"%s\" in tokens:\n",
                            routineName, optiontmp);
        *noption=0;
        pch = strtok(optiontmp," ,");
        while (pch != NULL) {
            poptionstr[*noption] = (string) malloc(10);
            strcpy(poptionstr[*noption],pch);
            ++(*noption);
            verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                                  "%s: %s\n",
                                routineName, poptionstr[*noption-1]);
            pch = strtok (NULL, " ,");
        }

        verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                               "%s: num. of tokens in option %s =%d\n",
                            routineName, optionstr, *noption);

        if (flag == 0)
            if (*noption != nfiles)
                error("\nscanOption: noption = %d %s",
                      *noption,
                      "must be equal to number of infiles\n\n");
        if (*noption > MAXITEMS)
            error("\nscaniOption: noption = %d %s",
                  *noption,
                  "must be less than the maximum num. of lines\n\n");

        for (i=0; i<*noption; i++) {
            option[i]=atoi(poptionstr[i]);
            verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                                   "%s: option: %d\n",
                                routineName, option[i]);
        }
        verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                               "%s: noptions, nfiles: %d %d\n\n",
                            routineName, *noption,nfiles);
        if (flag == 1) {
            if (*noption > nfiles)
                error("\nscanOption: noption = %d %s",
                      *noption,
                      "must be less or equal to number of files\n\n");
            else {
                for (i=*noption; i<nfiles; i++) {
                    option[i]=option[i-1]+1;
                    verb_print_debug_info(cmd->verbose,
                                          cmd->verbose_log, gd->outlog,
                                          "%s: option: %d\n",
                                          routineName, option[i]);
                }
            }
        }
    }

    return SUCCESS;
}

local int scanrOption(struct  cmdline_data* cmd, struct  global_data* gd,
                      string optionstr, double *option, int *noption,
    int nfiles, int flag, string message)
{
    string routineName = "scanrOption";

    char *pch;
    char *poptionstr[30],  optiontmp[100];
    int i;

    verb_log_print(cmd->verbose_log, gd->outlog,
                           "\n%s: Processing '%s' option:\n",
                        routineName, message);

    verb_log_print(cmd->verbose_log, gd->outlog,
                           "%s: Splitting string \"%s\" in tokens:\n",
                        routineName, message);

    if (!strnull(optionstr)) {
        strcpy(optiontmp,optionstr);
        verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                               "\n%s: Splitting string \"%s\" in tokens:\n",
                            routineName, optiontmp);
        *noption=0;
        pch = strtok(optiontmp," ,");
        while (pch != NULL) {
            poptionstr[*noption] = (string) malloc(MAXLENGTHOFREAL);
            strcpy(poptionstr[*noption],pch);
            ++(*noption);
            verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                                  "%s: %s\n",
                                routineName, poptionstr[*noption-1]);
            pch = strtok (NULL, " ,");
        }
        verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                              "%s: num. of tokens in option %s =%d\n",
                            routineName, optionstr, *noption);

        if (flag == 0)
            if (*noption != nfiles)
            error("\nscanOption: noption = %d must be equal to number of files\n\n",
                      *noption);
        if (*noption > MAXITEMS)
    error("\nscanOption: noption = %d must be less than the maximum num. of lines\n\n",
                  *noption);

        for (i=0; i<*noption; i++) {
            option[i]=atof(poptionstr[i]);
            if (cmd->verbose_log>=3)
            verb_log_print(cmd->verbose_log, gd->outlog, "option: %g\n",option[i]);
        }

        verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                              "\n%s: noptions, nfiles: %d %d\n",
                            routineName, *noption, nfiles);
        if (flag == 1) {
            if (*noption > nfiles)
    error("\nscanOption: noption = %d must be less or equal to number of files\n\n",
                  *noption);
            else {
                for (i=*noption; i<nfiles; i++) {
                    option[i]=option[i-1]+1;
                    if (cmd->verbose_log>=3)
                    verb_log_print(cmd->verbose_log, gd->outlog, 
                                   "option: %g\n",option[i]);
                }
            }
        }

        for (i=0; i<*noption; i++) {
            free(poptionstr[*noption]);
        }

    } else {
        for (i=0; i<nfiles; i++) {
            option[i]=0.0;                          // Be aware of this values
            verb_print_debug_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                                   "%s: option: %d\n",
                                routineName, option[i]);
        }
    }

    return SUCCESS;
}

//E Special routines to scan command line

local int print_make_info(struct cmdline_data* cmd,
                          struct  global_data* gd)
{
    verb_print(cmd->verbose,
               "\nprint_make_info:\n");

#ifdef OPENMPCODE
    verb_print(cmd->verbose, "using OpenMP\n");
#endif

#ifdef SINGLEP
    verb_print(cmd->verbose, "SINGLEP\n");
#endif

#ifdef LONGINT
    verb_print(cmd->verbose, "LONGINT\n");
#endif

#ifdef DEBUG
    verb_print(cmd->verbose, "DEBUG\n");
#endif

#ifdef USEGSL
#ifdef GSLINTERNAL
    verb_print(cmd->verbose, "using internal GSL\n");
#else
    verb_print(cmd->verbose, "using GSL\n");
#endif
#endif

#ifdef ADDONS
    verb_print(cmd->verbose, "with ADDONS\n");
#endif

#ifdef CLASSLIB
    verb_print(cmd->verbose, "with CLASSLIB\n");
#endif

#ifdef PXD
    verb_print(cmd->verbose, "with PXD\n");
#endif

    return SUCCESS;
}

// I/O directories:
local void setFilesDirs_log(struct cmdline_data* cmd,
                             struct  global_data* gd)
{
    string routineName = "setFilesDirs_log";
    char buf[BUFFERSIZE];

    debug_tracking_s("001", routineName);
    if (cmd->verbose_log>0) {           // gd->logfilePath is defined
        debug_tracking_s("002", cmd->rootDir);
        sprintf(gd->tmpDir,"%s/%s",cmd->rootDir,"tmp");
        double cpustart = CPUTIME;
        debug_tracking_s("003", gd->tmpDir);
        sprintf(buf,"if [ ! -d %s ]; then mkdir %s; fi",
                gd->tmpDir,gd->tmpDir);
        system(buf);
        debug_tracking("004");
        gd->cputotalinout += CPUTIME - cpustart;
        sprintf(gd->logfilePath,"%s/wlcov.log",
                gd->tmpDir);
//        gd->tmpDir,cmd->suffixOutFiles);
    }
    debug_tracking("005... final");
}

local void setFilesDirs(struct cmdline_data* cmd, struct  global_data* gd)
{
    string routineName = "setFilesDirs";
    char buf[BUFFERSIZE];

    char outputDir[MAXLENGTHOFFILES];

    double cpustart = CPUTIME;

    int ndefault = 0;
    int *ipos;
    char *dp1, *dp2;
    int lenDir = strlen(cmd->rootDir);
    int i;

    debug_tracking_s("001", routineName);
    debug_tracking_s("002: init", cmd->rootDir);

    if (gd->rootDirFlag==TRUE) {
        
        int nslashs = MAXNSLASHS;
        ipos = (int*) malloc((nslashs)*sizeof(int));
//        dp1 = (char*) malloc((lenDir)*sizeof(char));
        dp1 = (char*) malloc((MAXLENGTHOFSTRSCMD)*sizeof(char));

        for (i=0; i< lenDir; i++) {
            if(cmd->rootDir[i] == '/') {
                ipos[ndefault] = i+1;
                ndefault++;
            }
        }
        if (ndefault>nslashs)
            error("%s: more '/' than %d in 'rootDir=%s'. Use only %d or none\n",
                  routineName, nslashs, cmd->rootDir, nslashs);
        
        if (ndefault == 0) {
            sprintf(outputDir,cmd->rootDir);
            sprintf(buf,"if [ ! -d %s ]; then mkdir %s; fi",
                    outputDir,outputDir);
            if (cmd->verbose >= 3)
                verb_print_q(3, cmd->verbose_log,"\nsystem: %s\n",buf);
            system(buf);
        } else {
            for (i=0; i<ndefault; i++) {
                debug_tracking("003");
//                strncpy(dp1, cmd->rootDir, ipos[i]-1);
                snprintf(dp1, ipos[i]+1, "%s", cmd->rootDir);
                sprintf(buf,"if [ ! -d %s ]; then mkdir -p %s; fi",dp1,dp1);
                verb_print_q(3,cmd->verbose_log,"\nsystem: %d: %s\n",i,buf);
                system(buf);
                debug_tracking("004");
            }
//            strncpy(dp1, cmd->rootDir, lenDir);
            snprintf(dp1, lenDir+1, "%s", cmd->rootDir);
            sprintf(buf,"if [ ! -d %s ]; then mkdir -p %s; fi",dp1,dp1);
            verb_print_q(3,cmd->verbose_log,"\nsystem: %d: %s\n",i,buf);
            system(buf);
        }
        gd->cputotalinout += CPUTIME - cpustart;
        
        debug_tracking_s("005", cmd->rootDir);
/*
        sprintf(gd->fpfnameOutputFileName,"%s/%s%s%s",
                cmd->rootDir,cmd->outfile,cmd->suffixOutFiles,EXTFILES);
        sprintf(gd->fpfnamehistNNFileName,"%s/%s%s%s",
                cmd->rootDir,cmd->histNNFileName,cmd->suffixOutFiles,EXTFILES);
        sprintf(gd->fpfnamehistCFFileName,"%s/%s%s%s",
                cmd->rootDir,"histCF",cmd->suffixOutFiles,EXTFILES);
        sprintf(gd->fpfnamehistrBinsFileName,"%s/%s%s%s",
                cmd->rootDir,"rbins",cmd->suffixOutFiles,EXTFILES);
        sprintf(gd->fpfnamehistXi2pcfFileName,"%s/%s",
                cmd->rootDir,cmd->histXi2pcfFileName);
        sprintf(gd->fpfnamehistZetaGFileName,"%s/%s%s%s",
                cmd->rootDir,cmd->histZetaFileName,"G",cmd->suffixOutFiles);
        sprintf(gd->fpfnamehistZetaGmFileName,"%s/%s%s%s",
                cmd->rootDir,cmd->histZetaFileName,"G",cmd->suffixOutFiles);
        sprintf(gd->fpfnamehistZetaMFileName,"%s/%s%s%s",
                cmd->rootDir,cmd->histZetaFileName,"M",cmd->suffixOutFiles);
        sprintf(gd->fpfnamemhistZetaMFileName,"%s/%s%s%s%s",
                cmd->rootDir,"m",cmd->histZetaFileName,"M",cmd->suffixOutFiles);
        sprintf(gd->fpfnameCPUFileName,"%s/cputime%s%s",
                cmd->rootDir,cmd->suffixOutFiles,EXTFILES);
*/
        free(ipos);
        

        //B socket:
#ifdef ADDONS
#include "wlcfio_include_09b.h"
#endif
        //E
        free(dp1);
    } // ! rootDirFlag
    debug_tracking_s("006: final", cmd->rootDir);
}

int StartOutput(struct cmdline_data *cmd, struct  global_data* gd)
{
    //B clear some char arrays
//    gd->logfilePath[0] = '\0';
//    gd->fpfnameOutputFileName[0] = '\0';
//    gd->fnameData_kd[0] = '\0';
//    gd->fnameOut_kd[0] = '\0';
    //E

//    outfilefmt_string_to_int(cmd->outfilefmt, &outfilefmt_int);

    if (cmd->verbose>=VERBOSEMININFO)
        if (! strnull(cmd->options))
            verb_print(cmd->verbose, "\n\toptions: %s\n", cmd->options);

    return SUCCESS;
}
