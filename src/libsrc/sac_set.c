#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dbg.h"
#include "sac.h"
#include "sacutil.h"


int sac_set_data(sac *tr, float *data, int npts)
/* modify data in sac 
 *
 */
{
  int i=0;
  float *fpt=NULL;

  /* (re)allocate space */
  if (tr->data != NULL) {
    fpt = (float *)realloc(tr->data, npts*sizeof(float));
  } else {
    fpt = (float *)calloc(npts, sizeof(float));
  }
  check_mem(fpt);
  tr->data = fpt;

  /* copy data */
  for (i=0;i<npts;i++) tr->data[i] = data[i];

  /* modify header */
  tr->npts = npts;

  return 0;

error:
  return -1;
}


int sac_set_epoch(sac *tr)
/* set Epoch time for sac
 * sac header accessed: nzyear,nzjday,nzhour,nzmin,nzsecond
 * sac header modified: epoch
 */
{
  long eptime;
  struct tm sacgmt;

  /* set tm from sac */
  sacgmt.tm_year=tr->nzyear-1900;
  sacgmt.tm_mon=0;
  sacgmt.tm_mday=tr->nzjday; /* mktime can handle this properly */
  sacgmt.tm_hour=tr->nzhour;
  sacgmt.tm_min=tr->nzmin;
  sacgmt.tm_sec=tr->nzsec;

  debug("gmt=%s",asctime(&sacgmt));

  /* compute epoch time */
  setenv("TZ","UTC",1);
  tzset(); 
  eptime = mktime(&sacgmt);
  /* FIXME set time zone back */
  check(eptime!=-1,
    "Error setting epoch time from %s", asctime(&sacgmt));

  debug("gmt=%s",asctime(&sacgmt));

  /* relative begin time */
  /*
  tb_sec = tr->b;
  tb_usec = 1000000*((double)tr->b - tb_sec); \*microsecond*\
  */

  /* set sac header field */
  tr->epoch.tv_sec = eptime;
  tr->epoch.tv_usec = tr->nzmsec*1000;

  return 0;

error:
  return -1;
}


int sac_set_epochb(sac *tr)
/* set Epoch time for the first sample in sac 
 * sac header accessed: b,nzyear,nzjday,nzhour,nzmin,nzsec,nzmsec
 * sac header modified: epoch
 */
{
  long eptime, tb_sec, tb_usec;
  double flterr;
  struct tm sacgmt;

  /* set tm from sac */
  sacgmt.tm_year=tr->nzyear-1900;
  sacgmt.tm_mon=0;
  sacgmt.tm_mday=tr->nzjday; /* mktime can handle this properly */
  sacgmt.tm_hour=tr->nzhour;
  sacgmt.tm_min=tr->nzmin;
  sacgmt.tm_sec=tr->nzsec;

  debug("gmt=%s",asctime(&sacgmt));

  /* compute epoch time */
  setenv("TZ","UTC",1);
  tzset(); 
  eptime = mktime(&sacgmt);
  /* FIXME set time zone back */
  check(eptime!=-1,
    "Error setting epoch time from %s", asctime(&sacgmt));

  debug("gmt=%s",asctime(&sacgmt));

  /* begin time */
  /* for 32-bit floaing point number, the precision is 2^-24 = 6e-8 
   * also, notice for GPS cycle mode (automatic), the timing precision
   * is about 100us(1e-4 s). 
   */
  flterr = (double)tr->b*6e-8;
  if (flterr>1e-4)
    log_warn ("Floating point error of sac.b (%f) is larger than 100us", 
              flterr);

  tb_sec = tr->b;
  tb_usec = 1000000*(tr->b-tb_sec); /*microsecond*/

  /* set sac header field */
  tr->epoch.tv_sec = eptime+tb_sec;
  tr->epoch.tv_usec = tr->nzmsec*1000+tb_usec;

  return 0;

error:
  return -1;
}
