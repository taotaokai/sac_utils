#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "dbg.h"
#include "sac.h"
#include "sac_utils.h"

/*****************************************/

sac * sac_new(void)
/* creat sac struct */
{
  /* allocate space */
  sac *tr=NULL;
  tr = (sac *)calloc(1,sizeof(sac));
  check_mem(tr);

  /* initialize */
  tr->npts = 0;
  tr->iftype = 1;
  tr->nvhdr = 6;

  return tr;

error:
  return NULL;
}

/*****************************************/

sac * sac_new_n(int ntr)
/* creat sac struct array */
{
    int i=0;

    /* allocate space */
    sac *tr=NULL;
    tr = (sac *)calloc(ntr, sizeof(sac));
    check_mem(tr);

    /* initialize */
    for (i=0;i<ntr;i++) {
        tr[i].npts = 0;
        tr[i].iftype = 1;
        tr[i].nvhdr = 6;
    }

    return tr;

error:
    return NULL;
}

/*****************************************/

void sac_free(sac *tr)
/* free sac struct */
{
  int i=0;

  /* free memories */
  if (tr) 
  {
    if(tr[i].data) { free(tr[i].data); }
    free(tr);
  }
}

/*****************************************/

void sac_free_n(sac *tr, int ntr)
/* free sac struct array */
{
  int i=0;

  /* free memories */
  if (tr) 
  {
    for (i=0; i<ntr; i++) 
    {
      if(tr[i].data) { free(tr[i].data); }
    }
    free(tr);
  }
}

/*****************************************/

int sac_io_isSAC(char *filename)
/* check if a valid SAC file
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

/*****************************************/

int sac_io_read(sac *tr, char *sacfile)
/* read one sac file */
{
	int ret_code; /* return code */
	float *fpt=NULL;

    /* validate sac file */
    check(sac_io_isSAC(sacfile)==0, "ERROR: %s is not sac file", sacfile);

	/* open sac file */
	FILE *fp; 
	fp = fopen(sacfile, "rb");
	check(fp != NULL, "Error open file: %s",sacfile);	

	/* read header field */
	ret_code = fread(tr, SAC_HEADER_SIZE, 1, fp);
	check(ret_code==1, "Error read in sac header");
	
	/* allocate space for data */
	if (tr->data != NULL) {
		fpt = (float *)realloc(tr->data, tr->npts*sizeof(float));	
	} else {
		fpt = (float *)calloc(tr->npts, sizeof(float));
	}
	check_mem(fpt);
	tr->data = fpt;

	/* read data field  */
	ret_code = fread(tr->data, tr->npts*sizeof(float), 1, fp);
	check(ret_code==1, "Error read in sac data");

	fclose(fp);
	return 0;

error:
	if(fp) fclose(fp);
	return -1;
}

/*****************************************/

int sac_io_read_n(sac *tr, char **sacfile, int ntr)
/* read a list of sac files */
{
	int ret_code;

	/* read each sac file */
	int i=0;
	for (i=0; i<ntr; i++) {
		ret_code = sac_io_read(&tr[i], sacfile[i]);
		check(ret_code==0, "Error read sacfile[%d]=%s",i,sacfile[i]);
	}
	
	return 0;

error:
	return -1;
}


/*****************************************/

int sac_io_write(sac *tr, char *sacfile)
/* output one sac file */
{
	int ret_code;

	/* open sac file */
	FILE *fp; 
	fp = fopen(sacfile, "w");
	check(fp != NULL, "Error open file: %s",sacfile);	

	/* write header field */
	ret_code = fwrite(tr, SAC_HEADER_SIZE, 1, fp);
	check(ret_code==1, "Error write sac header");
	
	/* write data field */
	ret_code = fwrite(tr->data, tr->npts*sizeof(float), 1, fp);
	check(ret_code==1, "Error write sac data");

	fclose(fp);
	return 0;

error:
	if (fp) fclose(fp);
	return -1;
}

/*****************************************/

int sac_io_write_n(sac *tr, char **sacfile, int ntr)
/* output sac struct array into a list of files */
{
	int i, ret_code=-1;

	/* write each sac file */
	for (i=0; i<ntr; i++) {
		ret_code = sac_io_write(&tr[i], sacfile[i]);
		check(ret_code==0, "Error write sacfile[%d]=%s",i,sacfile[i]);
	}
	
	return 0;

error:
	return -1;
}
