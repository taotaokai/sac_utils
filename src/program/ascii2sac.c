/* ASCII2SAC: $Revision: 0.0 $; $Date: 2014/09/16 16:25:00 $ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "dbg.h"
#include "sac.h"
#include "sacutil.h"
#include "par.h"

/*********************** self documentation ******************************/
char *sdoc[] = {
"NAME ",
"  ascii2sac ",
" ",
"DESCRIPTION ",
"  read from stdin a stream of float numbers and ",
"  write out a SAC file",
" ",
"SYNOPSIS ",
"  sacsethd < in.txt [<header name>=<value>] ... ",
" ",
"COMMENTS ",
"  default sac header values: b=0 delta=1 ",
"  npts and e are set automatically ",
" ",
"PARAMETERS ",
" ",
"  <header name>=<value>   set the corresponding header value",
" ",
NULL};
/**************** end self doc *******************************************/

#define MAXPAR 100

int main(int argc, char *argv[])
{
  int i, npar;
  int hdidx, hdpos, hdsize;

  char *parname[MAXPAR]={NULL};
  char *asciival[MAXPAR]={NULL};
  char *out=NULL;

  union _HDVAL_
  {
    int i;
    float f;
    long l;
    char c[16];
  } hdval;

  sac *tr=NULL;
  char *tail=NULL;
  int npts, arraysize;
  float *data=NULL;

  /*=== Get parameters ===*/
  initargs(argc, argv);
  requestdoc(1);

  /* get parameter names and ascii values */
  npar = getargtbl(parname, asciival, MAXPAR);
  /*FIXME: should automatic increase size instead of MAXPAR */

  if (!getparstring("out", &out)) out="out.sac";

  /*=== Set SAC header values ===*/
  tr = sacnewn(1);

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
        memcpy((char *)tr+hdpos, &hdval, hdsize);
      }
      else {
        log_err("Overflow occured for %s", parname[i]);
      }

    } /* if (hdidx>=0) */
  } /* for par */


  /*=== Read numbers from stdin into an array ===*/
  npts=0;
  arraysize=1;
  check_mem(data = malloc(sizeof(float)*arraysize));
  while (scanf("%f", &data[npts]) == 1) {
    debug("data[%d]=%f", npts, data[npts]);
    npts++;
    if (npts>=arraysize) {
      arraysize = 2*arraysize;
      debug("arraysize=%d",arraysize);
      check_mem(data = realloc(data, sizeof(float)*arraysize));
    }
  }
  debug("npts=%d",npts);

  /*=== Write out sac file ===*/
  tr->data=data;
  tr->npts=npts;
  if (!getparfloat("delta", &tr->delta)) tr->delta=1.0;
  if (!getparfloat("b", &tr->b)) tr->b=0.0;
  tr->e = tr->b + npts*tr->delta;
  check(sacwrite(tr, out)==0, "Error write sac: %s", out);


  /*=== Clean up ===*/
  sacfreen(tr,1);
  return 0;

error:
  if (data) free(data);
  tr->data = NULL;
  sacfreen(tr,1);
  return -1;
}
