#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "dbg.h"
#include "sac.h"
#include "sacutil.h"

int sacchk(char *filename)
/* Check if filename a valid sac file
 *
 * Return value
 *  0 if a valid sac file, else -1
 *
 */
{
  int ret_code;
  struct stat filestat;
  long fsize;
  FILE *fp=NULL;
  int nvhdr;

  /* test if file exists */
  ret_code = stat(filename, &filestat);
  check(ret_code==0, "%s does not exist", filename);
  
  /* test if sac file by file size */
  fsize = filestat.st_size;
  check((fsize>=SAC_HEADER_SIZE) && ((fsize-SAC_HEADER_SIZE)%4 == 0),
        "%s is not a sac file, size=%ld", filename, fsize);

  /* test by header content */
  fp = fopen(filename, "rb");
  fseek(fp, 304, SEEK_SET);
  fread(&nvhdr, sizeof(int), 1, fp);
  check(nvhdr==6, "%s is not a sac file, nvhdr=%d", filename, nvhdr);
  fclose(fp);

  return 0;

error:
  if (fp) fclose(fp);
  return -1;
}
