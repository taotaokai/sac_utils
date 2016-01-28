#include <stdio.h>
#include <stdlib.h>
#include <strings.h> /* For POSIX:strcasecmp() */
#include <math.h> /* For macro NAN */

#include "dbg.h"
#include "sac.h"
#include "sacutil.h"


int sac_get_head_index(char *headername)
/* find index value such that SACHD[index].hdnm = <headername>
 *
 * Parameters
 *   header_name - pointer to a null-terminated character string
 *
 * Return value
 *   if no error occur, index of the header is returned.
 *
 * Error handling
 *   if not a valid headername, -1 is returned.
 */
{
  int i;
  for (i=0; i<SAC_HEADER_FIELDS; i++) {
    if (strcasecmp(headername, SACHD[i].hdnm)==0)
      return i;
  }

  sentinel("Invalid headername: %s",headername);

error:
  return -1;
}


float sac_get_fhv(sac *tr, char *headername)
/* get value of float type header field 
 *
 * Parameters
 *   tr - pointer to a sac struct
 *   headername -  pointer to a character string
 *
 * Return value
 *   if no error occur, the value of the headername is returned.
 *
 * Error handling
 *   if not a valid float headername, NAN is returned.
 */
{
  int idx=sac_get_head_index(headername);

  /* check if a float header */
  check (idx>=SAC_HEADER_FLOAT_MIN && idx<=SAC_HEADER_FLOAT_MAX,
    "[%s] is not a float header", headername);

  return *((float *)tr+idx);

error:
  return NAN;
}


int sac_get_tm(sac *tr, struct tm *sacgmt)
/* get broke-down time representation of sac reference time 
 * sac header accessed: nzyear,nzjday,nzhour,nzmin,nzsec
 */
{
  time_t eptime;

  /* set tm from sac */
  sacgmt->tm_year=tr->nzyear-1900;
  sacgmt->tm_mon=0;
  sacgmt->tm_mday=tr->nzjday; /* mktime can handle this properly */
  sacgmt->tm_hour=tr->nzhour;
  sacgmt->tm_min=tr->nzmin;
  sacgmt->tm_sec=tr->nzsec;
  sacgmt->tm_isdst=0;

  /* compute epoch time */
  setenv("TZ","UTC",1);
  tzset();
  eptime = mktime(sacgmt);
  check(eptime!=-1,
    "Error setting epoch time from %s", asctime(sacgmt));

  return 0;

error:
  return -1;
}
