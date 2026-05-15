/*==============================================================================
 * input.c module.
 *
 * Julien Lesgourgues, 27.08.2010
 *
 * Adapted to be used in cTreeBalls by Mario A. Rodriguez-Meza
==============================================================================*/
//        1          2          3          4        ^ 5          6          7

//
// lines where there is a "//B socket:" string are places to include module files
//  that can be found in addons/addons_include folder
//

#include "globaldefs.h"
#include "input.h"

local void testParameterFile(struct  cmdline_data*,
                             struct  global_data*,
                             char *);

int input_find_file(struct  cmdline_data* cmd, struct  global_data* gd,
                    char *fname,
                    struct file_content * fc,
                    ErrorMsg errmsg){

  struct file_content fc_input;
  struct file_content fc_precision;
  struct file_content * pfc_input;
  struct file_content fc_setroot;

  int i;
  char extension[5];
  char input_file[_ARGUMENT_LENGTH_MAX_];
  char precision_file[_ARGUMENT_LENGTH_MAX_];

    //B test if cmd->paramfile exist...
    if (!strnull(fname)) {
        testParameterFile(cmd, gd, cmd->paramfile);
    }
    //E

  pfc_input = &fc_input;

  fc->size = 0;
  fc_input.size = 0;
  fc_precision.size = 0;
  input_file[0]='\0';
  precision_file[0]='\0';

    stream outstr;
    if (strnull(fname)) {
        verb_print(1,
        "If you intend to use a parameter file call with <ParameterFileName>\n");
        return FAILURE;
    } else {
        outstr = stropen(fname, "r");
        fclose(outstr);
    }

    strcpy(input_file,fname);

    if (!strnull(input_file)) {
        class_call(parser_read_file(input_file,&fc_input,errmsg),
                   errmsg, errmsg);
        class_call(input_set_root(input_file,&pfc_input,&fc_setroot,errmsg),
               errmsg, errmsg);
  }

  if ((input_file[0]!='\0') || (precision_file[0]!='\0')){
      class_call(parser_cat(pfc_input, &fc_precision, fc, errmsg),
                 errmsg, errmsg);
  }

  class_call(parser_free(pfc_input), errmsg, errmsg);
  class_call(parser_free(&fc_precision), errmsg, errmsg);

  return SUCCESS;
}

int input_set_root(char* input_file,
                   struct file_content** ppfc_input,
                   struct file_content * pfc_setroot,
                   ErrorMsg errmsg) {

  int flag1, filenum, iextens;
  int index_root_in_fc_input = -1;
  int overwrite_root;
  int found_filenum;

  FileArg outfname;

  struct file_content fc_root;                      // Temporary structure with
                                                    //  only the root name
  FileArg string1;                                  //  Is ignored
  struct file_content * pfc = *ppfc_input;

  class_call(parser_read_string(pfc,"rootDir",&string1,&flag1,errmsg),
             errmsg, errmsg);

//B To behave as not class_lib parameter file
    overwrite_root = TRUE;
    class_read_flag("overwrite_root",overwrite_root);
    overwrite_root = TRUE;
//E

    if (flag1 == FALSE){
        memcpy(outfname, "Output", 7);
    } else {
        for (index_root_in_fc_input=0;index_root_in_fc_input<pfc->size;
             ++index_root_in_fc_input) {
            if(strcmp(pfc->name[index_root_in_fc_input],"rootDir") == 0){
                strcpy(outfname,pfc->value[index_root_in_fc_input]);
                break;
            }
        }
    }
    if(flag1 == FALSE) {
        class_call(parser_init(&fc_root, 1, pfc->filename, errmsg),
                   errmsg,errmsg);
        sprintf(fc_root.name[0],"rootDir");
        sprintf(fc_root.value[0],"%s",outfname);
        fc_root.read[0] = FALSE;
        class_call(parser_cat(pfc, &fc_root, pfc_setroot, errmsg),
                   errmsg, errmsg);
        class_call(parser_free(pfc), errmsg, errmsg);
        class_call(parser_free(&fc_root), errmsg, errmsg);
        (*ppfc_input) = pfc_setroot;
    } else {
        sprintf(pfc->value[index_root_in_fc_input],"%s",outfname);
        (*ppfc_input) = pfc;
    }

  return SUCCESS;
}

int input_read_from_file(struct cmdline_data *cmd, struct  global_data* gd,
                         struct file_content * pfc,
                         ErrorMsg errmsg)
{
    int input_verbose = 0;

    gd->cmd_allocated = FALSE;

    class_read_int("verbose",input_verbose);
    verb_print(input_verbose, "\nReading input parameters...\n");

    class_call(input_read_parameters(cmd, gd, pfc, errmsg),errmsg,errmsg);

    return SUCCESS;
}

int input_read_parameters(struct cmdline_data *cmd,
                          struct  global_data* gd,
                          struct file_content * pfc,
                          ErrorMsg errmsg)
{
    int input_verbose=0;

    class_call(input_default_params(cmd),errmsg,errmsg);
    class_read_int("input_verbose",input_verbose);
    class_call(input_read_parameters_general(cmd, gd,  pfc,errmsg),errmsg,errmsg);

    return SUCCESS;
}

int input_read_parameters_general_free(struct file_content * pfc,
                                       ErrorMsg errmsg) {
}

int input_read_parameters_general(struct cmdline_data *cmd,
                                  struct  global_data* gd,
                                  struct file_content * pfc, ErrorMsg errmsg)
{
    string routineName = "input_read_parameters_general";
    int flag;
    int flag1,flag2;
    int param;
    int index;
    size_t slen;
    double param1,param2;
    char string1[_ARGUMENT_LENGTH_MAX_];

    // All malloc have to be freed at the end of the run (EndRun)
    debug_tracking_s("001", routineName);

    //B Parameters about the I/O file(s)
    // Input catalog parameters
    class_call(parser_read_string(pfc,"inputfile",&string1,&flag1,errmsg),
               errmsg,errmsg);
    gd->inputfileFlag=FALSE;
    if (flag1 == TRUE) {
        for (index=0;index<pfc->size;++index){
          if (strcmp(pfc->name[index],"inputfile") == 0){
              cmd->inputfile = (char*) malloc(MAXLENGTHOFSTRSCMD*sizeof(char));
              strcpy(cmd->inputfile,pfc->value[index]);
              gd->inputfileFlag=TRUE;
            break;
          }
        }
    }

    // Output parameters
    class_call(parser_read_string(pfc,"rootDir",&string1,&flag1,errmsg),
               errmsg,errmsg);
    gd->rootDirFlagFree=FALSE;
    if (flag1 == TRUE) {
        for (index=0;index<pfc->size;++index){
          if (strcmp(pfc->name[index],"rootDir") == 0){
              slen = strlen(pfc->value[index]);
              cmd->rootDir = (char*) malloc(MAXLENGTHOFSTRSCMD*sizeof(char));
              snprintf(cmd->rootDir, slen+1,
                       "%s", pfc->value[index]);
              debug_tracking_s("001: input rootDir", cmd->rootDir);
              gd->rootDirFlagFree=TRUE;
            break;
          }
        }
    }
    //E

    class_call(parser_read_double(pfc,"r",&param1,&flag1,errmsg),
               errmsg,errmsg);
    if (flag1 == TRUE){
      cmd->r = param1;
    }
    class_call(parser_read_double(pfc,"theta1",&param1,&flag1,errmsg),
               errmsg,errmsg);
    if (flag1 == TRUE){
      cmd->theta1 = param1;
    }
    class_call(parser_read_double(pfc,"thetap1",&param1,&flag1,errmsg),
               errmsg,errmsg);
    if (flag1 == TRUE){
      cmd->thetap1 = param1;
    }
    class_call(parser_read_double(pfc,"theta2",&param1,&flag1,errmsg),
               errmsg,errmsg);
    if (flag1 == TRUE){
      cmd->theta2 = param1;
    }
    class_call(parser_read_double(pfc,"thetap2",&param1,&flag1,errmsg),
               errmsg,errmsg);
    if (flag1 == TRUE){
      cmd->thetap2 = param1;
    }

    class_call(parser_read_int(pfc,"m",&param,&flag,errmsg), errmsg,errmsg);
    if (flag == TRUE) cmd->m = param;
    class_call(parser_read_int(pfc,"mp",&param,&flag,errmsg), errmsg,errmsg);
    if (flag == TRUE) cmd->mp = param;
    class_call(parser_read_int(pfc,"ppp",&param,&flag,errmsg), errmsg,errmsg);
    if (flag == TRUE) cmd->ppp = param;

    class_call(parser_read_double(pfc,"ellmax",&param1,&flag1,errmsg),
               errmsg,errmsg);
    if (flag1 == TRUE){
      cmd->ellmax = param1;
    }
    class_call(parser_read_double(pfc,"ellmin",&param1,&flag1,errmsg),
               errmsg,errmsg);
    if (flag1 == TRUE){
      cmd->ellmin = param1;
    }

    class_call(parser_read_int(pfc,"verbose",&param,&flag,errmsg),errmsg,errmsg);
    if (flag == TRUE)
      cmd->verbose = param;
    class_call(parser_read_int(pfc,"verbose_log",&param,&flag,errmsg),
               errmsg,errmsg);
    if (flag == TRUE)
      cmd->verbose_log = param;

#ifdef OPENMPCODE
    class_call(parser_read_int(pfc,"numberThreads",&param,&flag,errmsg),
               errmsg,errmsg);
    if (flag == TRUE)
      cmd->numthreads = param;
#endif

    class_call(parser_read_string(pfc,"options",&string1,&flag1,errmsg),
               errmsg,errmsg);
    gd->optionsFlag=FALSE;
    if (flag1 == TRUE) {
        for (index=0;index<pfc->size;++index){
          if (strcmp(pfc->name[index],"options") == 0){
              cmd->options = (char*) malloc(MAXLENGTHOFSTRSCMD);
              strcpy(cmd->options,pfc->value[index]);
              gd->optionsFlag=TRUE;
            break;
          }
        }
    }
    //E

//B socket:
#ifdef ADDONS
#include "class_lib_include_01.h"
#endif
//


  return SUCCESS;
}

//B cTreeBalls default values
int input_default_params(struct cmdline_data *cmd)
{
// Every item in cmdline_defs.h must have an item here::

    cmd->r = 0.116355;
    cmd->theta1 = 0.1;
    cmd->theta2 = 0.1;
    cmd->thetap1 = 0.1;
    cmd->thetap2 = 0.1;

    //B Parameters about the I/O file(s)
    // Input parameters
    cmd->inputfile = "CkappaT_ep.dat";
    // Output parameters
    cmd->rootDir = "Output";
    //E

    //B Parameters to control histograms and their output files
    cmd->ellmin = 1;
    cmd->ellmax = 2000.0;
    //E

    //B Set of parameters needed to construct a test model
//    cmd->seed=123;                                          // to always have
                                                        // defaults Check in gsl
    //E

    //B Miscellaneous parameters
    cmd->verbose = 0;
    cmd->verbose_log = 0;
#ifdef OPENMPCODE
    cmd->numthreads = 16;
#endif
    cmd->options = "\0";
    //E

//B socket:
#ifdef ADDONS
#include "class_lib_include_02.h"
#endif
//

  return SUCCESS;
}
//E


//B parameter reading/testing from a file
local void testParameterFile(struct  cmdline_data* cmd,
                             struct  global_data* gd,
                             char *fname)
{
// Every item in cmdline_defs.h must have an item here::
#define DOUBLE 1
#define STRING 2
#define INT 3
#define LONG 6
#define BOOLEAN 4
#define MAXTAGS 300
#define MAXCHARBUF 1024

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
    RPName(cmd->theta2,"theta1");
    RPName(cmd->thetap2,"thetap1");
    //E

    //B Parameters to control the I/O file(s)
    // Input catalog parameters
    SPName(cmd->inputfile,"inputfile",MAXLENGTHOFSTRSCMD);
    // Output parameters
    SPName(cmd->rootDir,"rootDir",MAXLENGTHOFSTRSCMD);
    //E
    IPName(cmd->m,"m");
    IPName(cmd->mp,"mp");
    IPName(cmd->ppp,"ppp");

    //B Set of parameters needed to integrate
    RPName(cmd->ellmax,"ellmin");
    RPName(cmd->ellmax,"ellmax");
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
                fprintf(stdout, "Error in file %s: Tag '%s' %s\n",
                        fname, name,
                        "not allowed or multiple defined...\n");
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
        error("\ntestParameterFile: going out\n");

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
            errorFlag=3;
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
