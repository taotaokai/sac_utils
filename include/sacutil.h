/* This is a Cfunctions (version 0.28) generated header file.
   Cfunctions is a free program for extracting headers from C files.
   Get Cfunctions from 'http://www.lemoda.net/cfunctions/'. */

/* This file was generated with:
'cfunctions --backup -n -g ../include/sacutil sacalloc.c sacchk.c sacget.c 
sacglob.c sacio.c sacproc.c sacset.c' */
#ifndef CFH_SACUTIL_H
#define CFH_SACUTIL_H

/* From 'sacalloc.c': */

#line 9 "sacalloc.c"
sac * sacnewn (int ntr );

#line 32 "sacalloc.c"
void sacfreen (sac *tr , int ntr );
/* From 'sacchk.c': */

#line 16 "sacchk.c"
int sacchk (char *filename );
/* From 'sacget.c': */

#line 23 "sacget.c"
int sacgethdidx (char *headername );

#line 50 "sacget.c"
float sacgetfhv (sac *tr , char *headername );

#line 68 "sacget.c"
int sac_get_tm (sac *tr , struct tm *sacgmt );
/* From 'sacglob.c': */

#line 141 "sacglob.c"
extern struct _SACHD_ SACHD[];
/* From 'sacio.c': */

#line 11 "sacio.c"
int sacread (sac *tr , char *sacfile );

#line 48 "sacio.c"
int sacwrite (sac *tr , char *sacfile );

#line 75 "sacio.c"
int sacreadn (sac *tr , char **sacfile , int ntr );

#line 94 "sacio.c"
int sacwriten (sac *tr , char **sacfile , int ntr );
/* From 'sacproc.c': */

#line 13 "sacproc.c"
int saccut (sac *tr , float b , float e );

#line 70 "sacproc.c"
int saccutepochb (sac *tr , long t0 , long t1 );
/* From 'sacset.c': */

#line 14 "sacset.c"
int sacsetdata (sac *tr , float *data , int npts );

#line 45 "sacset.c"
int sacsetepoch (sac *tr );

#line 91 "sacset.c"
int sacsetepochb (sac *tr );

#endif /* CFH_SACUTIL_H */
