/* SACSETHD: $Revision: 0.0 $; $Date: 2014/09/16 16:25:00 $ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "dbg.h"
#include "sac.h"
#include "sac_utils.h"
#include "getpar.h"

/*********************** self documentation ******************************/
char *sdoc[] = {
" ",
" SACSETHD - set/modify sac header values",
" ",
"   sacsethd [<header name>=<value>] ...", 
" ",
" ",
" Parameters:",
" ",
"   <header name>=<value>   set/modify the header value",
" ",
" Note: ",
" ",
"   The sac file names are read from stdin, such as ls *.sac | sacsethd ...",
" ",
" ",
" References:",
" ",
NULL};
/**************** end self doc *******************************************/

#define MAXPAR 100

int main(int argc, char *argv[])
{
  int i, npar, ret_code;
  int hdidx, hdpos, hdsize;

  char *parname[MAXPAR]={NULL};
  char *asciival[MAXPAR]={NULL};

  char *sacfn=NULL;
  size_t buflen=0;
  int len=0;

  FILE *fp=NULL;

  char *tail;
  union _HDVAL_
  {
    int i;
    float f;
    long l;
    char c[16];
  } hdval;

  /*=== Get parameters ===*/

  /* initialize */
  initargs(argc, argv);
  requestdoc(1);

  /* get parameter names and ascii values */
  npar = getargtbl(parname, asciival, MAXPAR);
  if (npar==0) {
    printf("no valid parameter specified\n");
    return -1;
  }

  /*=== Modify sac header ===*/
  while ((len=getline(&sacfn, &buflen, stdin)) >= 0) {

    /* delete trailing newline */
    if (sacfn[len-1]=='\n')
      sacfn[len-1] = 0;

    /* skip non-sac file */
    ret_code = sacchk(sacfn);
    if (ret_code != 0) {
      continue;
    }

    /* set/modify header */
    fp = fopen(sacfn, "r+b");

    for (i=0; i<npar; i++) {
      hdidx = sacgethdidx(parname[i]);

      if (hdidx>=0) { /* if a valid sac header name */
        hdpos = SACHD[hdidx].pos;
        hdsize = SACHD[hdidx].size;
        errno=0;
        memset(&hdval, 0, sizeof(hdval));
        switch (SACHD[hdidx].type) {
          case FLT32 : 
            hdval.f = strtof(asciival[i], &tail);
            break;
          case INT32 :
            hdval.i = strtol(asciival[i], &tail, 0);
            if (hdval.i<INT_MIN || hdval.i>INT_MAX)
              errno = ERANGE;
            break;
          case INT64 :
            hdval.l = strtol(asciival[i], &tail, 0);
            break;
          case CHAR8 :
            strncpy((char *)&hdval, asciival[i], 8);
            break;
          case CHAR16 :
            strncpy((char *)&hdval, asciival[i], 16);
            break;
        }
        if (errno==0) { /* check overflow */
          fseek(fp, hdpos, SEEK_SET);
          fwrite(&hdval, hdsize, 1, fp);
        } else {
          log_err("%s: overflow for %s, not changed.", 
                  sacfn, parname[i]);
        }
      }
    } /* for par */

    fclose(fp);

  } /* while getline */


  /*=== clear up ===*/
  free(sacfn);

  return 0;
}
