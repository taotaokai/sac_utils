#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"
#include "sac.h"
#include "sacutil.h"


/* read sac from a single file */
int sacread(sac *tr, char *sacfile)
{
	int ret_code; /* return code */
	float *fpt=NULL;

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


/* write sac struct into a single sac file */
int sacwrite(sac *tr, char *sacfile)
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


/* read a list of sac files into sac array */
int sacreadn(sac *tr, char **sacfile, int ntr)
{
	int ret_code;

	/* read each sac file */
	int i=0;
	for (i=0; i<ntr; i++) {
		ret_code = sacread(&tr[i], sacfile[i]);
		check(ret_code==0, "Error read sacfile[%d]=%s",i,sacfile[i]);
	}
	
	return 0;

error:
	return -1;
}


/* write sac array into a list of files */
int sacwriten(sac *tr, char **sacfile, int ntr)
{
	int i, ret_code=-1;

	/* write each sac file */
	for (i=0; i<ntr; i++) {
		ret_code = sacwrite(&tr[i], sacfile[i]);
		check(ret_code==0, "Error write sacfile[%d]=%s",i,sacfile[i]);
	}
	
	return 0;

error:
	return -1;
}
