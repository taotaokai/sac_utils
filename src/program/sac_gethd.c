/* SACGETHD: $Revision: 0.0 $; $Date: 2014/09/18 16:25:00 $ */

#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"
#include "sac.h"
#include "sac_utils.h"
#include "getpar.h"

/*********************** self documentation ******************************/
char *sdoc[] = {
" ",
" SACGETHD - get sac header values",
" ",
"   sacgethd [<header name>] ...", 
" ",
" ",
" Parameters:",
" ",
"   <header name>   get the header value",
" ",
" Note: ",
" ",
"   The sac file names are read from stdin, such as ls *.sac | sacgethd ...",
" ",
" ",
" References:",
" ",
NULL};
/**************** end self doc *******************************************/


int main(int argc, char *argv[])
{
  int i, ret_code;
  int hdidx;

  char *sacfn=NULL;
  size_t buflen=0;
  int len=0;

  FILE *fp=NULL;

  union _HD_ {
    float f;
    int i;
    long l;
    char c[17]; /* +1 for null termination */
  } hdval;

  /* self document on demand */
  if (argc==1)
    pagedoc();


  /*=== Get sac header ===*/

  /* get sac file name from stdin */
  while ((len=getline(&sacfn, &buflen, stdin)) >= 0) {

    /* delete trailing newline */
    if (sacfn[len-1]=='\n')
      sacfn[len-1] = 0;

    /* skip non-sac file */
    ret_code = sacchk(sacfn);
    if (ret_code != 0) {
      continue;
    }

    /* get header */
    fp = fopen(sacfn, "r+b");

    printf("%s", sacfn);
    for (i=1; i<argc; i++) {
      hdidx = sacgethdidx(argv[i]);

      if (hdidx>=0) { /* if a valid sac header name */
        fseek(fp, SACHD[hdidx].pos, SEEK_SET);
        memset(&hdval, 0, sizeof(hdval));
        fread(&hdval, SACHD[hdidx].size, 1, fp);

        switch (SACHD[hdidx].type) {
          case FLT32 : 
            printf(" %11.5f", hdval.f);
            break;
          case INT32 :
            printf(" %10d", hdval.i);
            break;
          case INT64 :
            printf(" %10ld", hdval.l);
            break;
          case CHAR8 :
            printf(" %-8s", (char *)&hdval);
            break;
          case CHAR16 :
            printf(" %-16s", (char *)&hdval);
            break;
        }
      }
    } /* for par */
    printf("\n");

    fclose(fp);

  } /* while getline */


  /*=== clean up ===*/
  free(sacfn);

  return 0;
}
