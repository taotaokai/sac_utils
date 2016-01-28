/* This is a Cfunctions (version 0.28) generated header file.
   Cfunctions is a free program for extracting headers from C files.
   Get Cfunctions from 'http://www.lemoda.net/cfunctions/'. */

/* This file was generated with:
'cfunctions --backup -n -g sac_utils sac_cut.c sac_get.c sac_glob.c sac_io.c 
sac_set.c' */
#ifndef CFH_SAC_UTILS_H
#define CFH_SAC_UTILS_H

/* From 'sac_cut.c': */

#line 14 "sac_cut.c"
int sac_cut (sac *tr , float b , float e );

#line 71 "sac_cut.c"
int sac_cut_epochb (sac *tr , long t0 , long t1 );
/* From 'sac_get.c': */

#line 23 "sac_get.c"
int sac_get_head_index (char *headername );

#line 50 "sac_get.c"
float sac_get_fhv (sac *tr , char *headername );

#line 68 "sac_get.c"
int sac_get_tm (sac *tr , struct tm *sacgmt );
/* From 'sac_glob.c': */

#line 141 "sac_glob.c"
extern struct _SACHD_ SACHD[];
/* From 'sac_io.c': */

#line 13 "sac_io.c"
sac * sac_new (void);

#line 34 "sac_io.c"
sac * sac_new_n (int ntr );

#line 59 "sac_io.c"
void sac_free (sac *tr );

#line 74 "sac_io.c"
void sac_free_n (sac *tr , int ntr );

#line 97 "sac_io.c"
int sac_io_isSAC (char *filename );

#line 131 "sac_io.c"
int sac_io_read (sac *tr , char *sacfile );

#line 172 "sac_io.c"
int sac_io_read_n (sac *tr , char **sacfile , int ntr );

#line 193 "sac_io.c"
int sac_io_write (sac *tr , char *sacfile );

#line 221 "sac_io.c"
int sac_io_write_n (sac *tr , char **sacfile , int ntr );
/* From 'sac_set.c': */

#line 14 "sac_set.c"
int sac_set_data (sac *tr , float *data , int npts );

#line 45 "sac_set.c"
int sac_set_epoch (sac *tr );

#line 91 "sac_set.c"
int sac_set_epochb (sac *tr );

#endif /* CFH_SAC_UTILS_H */
