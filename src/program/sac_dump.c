/* sac2ascii: print sac data field within a given time window 

History:
[20140612]tao: created
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "dbg.h"
#include "sac.h"
#include "sacutil.h"

static char help[]= "command line arguments: <sacfile> [-w<t1>/<t2>]";

int main(int argc, char **argv)
{
  char *sacfn=NULL;

  char *twin=NULL;
  const char delimiters[]="/,:"; /* for parsing tWin using strsep */
  char *string=NULL; /* pointer content will be modified by strsep */
  char *tofree=NULL; /* keep the original pointer of string */
  float t1=-INFINITY,t2=INFINITY; /* time window [t1,t2] */

  sac *sac1=sacnewn(1);
  int opt=0;
  int i;

  /* parse arguments */
  while ((opt = getopt(argc,argv,"w:h")) != -1) {
    switch(opt) {
      case 'w':
        twin = optarg;
        break;
      case 'h':
        printf("%s\n",help);
        exit(1);
      default:
        printf("%s\n",help);
        exit(1);
    }
  }

  check(optind<argc,"Must specify sac file!\n%s",help);
  sacfn = argv[optind];

  /* load sac */
  check(sacread(sac1,sacfn)==0,"Error read %s",sacfn);

  /* time window cut */
  if (twin) {
    string = strdup(twin);
    tofree = string;
    t1 = atof(strsep(&string,delimiters));
    t2 = atof(strsep(&string,delimiters));
  }

  check(saccut(sac1,t1,t2) == 0,
    "Failed to cut %s between [%f,%f]",sacfn,t1,t2);

  /* print out */
  for (i=0;i<sac1->npts;i++) {
    printf("%5.2f %14.10f\n",sac1->b+i*sac1->delta,sac1->data[i]);
  }

  /* free memory */
  free(tofree);
  sacfreen(sac1,1);
  return 0;

error:
  free(tofree);
  if (sac1) sacfreen(sac1,1);  
  return -1;
}
