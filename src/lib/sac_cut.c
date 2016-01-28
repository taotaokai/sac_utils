#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dbg.h"
#include "sac.h"
#include "sac_utils.h"


int sac_cut(sac *tr, float b, float e)
/* cut data between [b,e]
 * 
 */
{
  int n1,n2,n3;
  float *newdata=NULL;
  int i, ret_code;

  /* validate cut range */
  check (b<e, "Invalid cut range [%f,%f]",b,e);
  check ((b<tr->e) && (e>tr->b), 
    "cut range [%f,%f] outside of data range [%f,%f]!",b,e,tr->b,tr->e);
  if (b<tr->b) {
    log_warn("change cut begin: %f --> %f",b,tr->b);
    b = tr->b;
  }
  if (e>tr->e) {
    log_warn("change cut end: %f --> %f",e,tr->e);
    e = tr->e;
  }

  /* calculate index of cut range */
  n1 = ceil((b-tr->b)/tr->delta);
  n2 = (e-tr->b)/tr->delta;
  check(n2>=n1, "no data point between index [%d,%d]",n1,n2);
  
  debug("sac_cut: cut range [%f,%f]; data range [%f,%f]",b,e,tr->b,tr->e);
  debug("sac_cut: cut index [%d,%d]; data index [0,%d]",n1,n2,tr->npts-1);

  /* get cut data array */
  n3 = n2-n1+1;
  newdata = (float *)malloc(sizeof(float)*n3);
  check_mem(newdata);

  for (i=n1;i<=n2;i++) newdata[i-n1] = tr->data[i];

  /* set new data */
  ret_code = sac_set_data(tr,newdata,n3);
  check(ret_code==0, "Error set data");

  /* modify headers */
  tr->b += n1*tr->delta;
  tr->e = tr->b + (n2-n1)*tr->delta;
  tr->npts = n3;

  free(newdata);
  return 0;

error:
  if (newdata) free(newdata);
  return -1;
}


int sac_cut_epochb(sac *tr, long t0, long t1)
/* cut data between [t0,t1], which are represented as epoch time (second)
 * this assumes tr->epoch is set to the time of the first sample.
 * NOTE: only cut data to integer seconds.
 * Header read/modified: nsps, epoch, b, e, npts
 */
{
  long i, n0,n1,nn;
  float *newdata=NULL;
  int status;

  /* calculate index of cut range */
  tr->nsps = round(1/tr->delta);
  nn = (tr->epoch.tv_usec * tr->nsps)/1000000;
  n0 = (t0-tr->epoch.tv_sec)*tr->nsps - nn;
  n1 = (t1-tr->epoch.tv_sec)*tr->nsps - nn;

  /* validate cut range */
  check (n1>=n0, "invalid cut range index [%ld,%ld]",n0,n1);

  check ((n0<tr->npts) && (n1>=0), 
    "no overlap between cut [%ld,%ld] and data [0,%d] index range",
    n0,n1,tr->npts-1);
  
  if (n0<0) {
    log_warn("(n0=%ld)<0, change to 0",n0);
    n0 = 0;
  }

  if (n1>=tr->npts) {
    log_warn("(n1=%ld)>=(tr->npts=%d), change to tr->npts-1",n1,tr->npts);
    n1 = tr->npts-1;
  }
  
  debug("sac_cut: cut index [%ld,%ld]; data index [0,%d]",n0,n1,tr->npts-1);

  /* get cut data array */
  nn = n1-n0+1;
  newdata = (float *)malloc(sizeof(float)*nn);
  check_mem(newdata);

  for (i=n0;i<=n1;i++) newdata[i-n0] = tr->data[i];

  /* set new data */
  status = sac_set_data(tr,newdata,nn);
  check(status==0, "Error set data");

  /* modify headers */
  tr->b += n0*tr->delta;
  tr->e = tr->b + (nn-1)*tr->delta;
  tr->npts = nn;

  /* set epoch for b */
  nn = n0/tr->nsps;
  tr->epoch.tv_sec += nn;
  tr->epoch.tv_usec += (n0-nn*tr->nsps)*1000000/tr->nsps;
  if (tr->epoch.tv_usec >= 1000000) {
    tr->epoch.tv_sec++;
    tr->epoch.tv_usec -= 1000000;
  }

  free(newdata);
  return 0;

error:
  if (newdata) free(newdata);
  return -1;
}
