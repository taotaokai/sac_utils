#ifndef SAC_H
#define SAC_H

#include <stdlib.h>
#include <time.h>

/* copied from sac-101.6a/inc/SacHeader.h, 
 * add an additional field (float*) data
 */

typedef struct _sac_
{
  float  delta;      /* RF time increment, sec    */
  float  depmin;     /*    minimum amplitude      */
  float  depmax;     /*    maximum amplitude      */
  float  scale;      /*    amplitude scale factor */
  float  odelta;     /*    observed time inc      */
  float  b;          /* RD initial value, time    */
  float  e;          /* RD final value, time      */
  float  o;          /*    event start, sec < nz. */
  float  a;          /*    1st arrival time       */
  float  fmt;        /*    internal use           */
  float  t0;         /*    user-defined time pick */
  float  t1;         /*    user-defined time pick */
  float  t2;         /*    user-defined time pick */
  float  t3;         /*    user-defined time pick */
  float  t4;         /*    user-defined time pick */
  float  t5;         /*    user-defined time pick */
  float  t6;         /*    user-defined time pick */
  float  t7;         /*    user-defined time pick */
  float  t8;         /*    user-defined time pick */
  float  t9;         /*    user-defined time pick */
  float  f;          /*    event end, sec > nz    */
  float  resp0;      /*    instrument respnse parm*/
  float  resp1;      /*    instrument respnse parm*/
  float  resp2;      /*    instrument respnse parm*/
  float  resp3;      /*    instrument respnse parm*/
  float  resp4;      /*    instrument respnse parm*/
  float  resp5;      /*    instrument respnse parm*/
  float  resp6;      /*    instrument respnse parm*/
  float  resp7;      /*    instrument respnse parm*/
  float  resp8;      /*    instrument respnse parm*/
  float  resp9;      /*    instrument respnse parm*/
  float  stla;       /*  T station latitude       */
  float  stlo;       /*  T station longitude      */
  float  stel;       /*  T station elevation, m   */
  float  stdp;       /*  T station depth, m       */
  float  evla;       /*    event latitude         */
  float  evlo;       /*    event longitude        */
  float  evel;       /*    event elevation        */
  float  evdp;       /*    event depth            */
  float  mag;        /*    reserved for future use*/
  float  user0;      /*    available to user      */
  float  user1;      /*    available to user      */
  float  user2;      /*    available to user      */
  float  user3;      /*    available to user      */
  float  user4;      /*    available to user      */
  float  user5;      /*    available to user      */
  float  user6;      /*    available to user      */
  float  user7;      /*    available to user      */
  float  user8;      /*    available to user      */
  float  user9;      /*    available to user      */
  float  dist;       /*    stn-event distance, km */
  float  az;         /*    event-stn azimuth      */
  float  baz;        /*    stn-event azimuth      */
  float  gcarc;      /*    stn-event dist, degrees*/
  float  sb;         /*    internal use           */
  float  sdelta;     /*    internal use           */
  float  depmen;     /*    mean value, amplitude  */
  float  cmpaz;      /*  T component azimuth      */
  float  cmpinc;     /*  T component inclination  */
  float  xminimum;   /*    reserved for future use*/
  float  xmaximum;   /*    reserved for future use*/
  float  yminimum;   /*    reserved for future use*/
  float  ymaximum;   /*    reserved for future use*/
/* used for local cartesian coordinate: stx,sty,stz; evx,evy,evz
  float  unused6;    \*    reserved for future use*\
  float  unused7;    \*    reserved for future use*\
  float  unused8;    \*    reserved for future use*\
  float  unused9;    \*    reserved for future use*\
  float  unused10;   \*    reserved for future use*\
  float  unused11;   \*    reserved for future use*\
*/
  float stx;         /* KT station x coordinate   */
  float sty;         /* KT station y coordinate   */
  float stz;         /* KT station z coordinate   */
  float evx;         /* KT event x coordinate     */
  float evy;         /* KT event y coordinate     */
  float evz;         /* KT event z coordinate     */
  float unused12;    /*    reserved for future use*/
  int   nzyear;      /*  F zero time of file, yr  */
  int   nzjday;      /*  F zero time of file, day */
  int   nzhour;      /*  F zero time of file, hr  */
  int   nzmin;       /*  F zero time of file, min */
  int   nzsec;       /*  F zero time of file, sec */
  int   nzmsec;      /*  F zero time of file, msec*/
  int   nvhdr;       /*    internal use           */
  int   norid;       /*    origin ID              */
  int   nevid;       /*    event ID               */
  int   npts;        /* RF number of samples      */
  int   nsnpts;      /*    internal use           */
  int   nwfid;       /*    waveform ID            */
  int   nxsize;      /*    reserved for future use*/
  int   nysize;      /*    reserved for future use*/
/* tao: used for sampling rate
  int   unused15;    \*    reserved for future use*\
*/
  int   nsps;        /* KT number of samples per second, sampling rate */
  int   iftype;      /* RA type of file           */
  int   idep;        /*    type of amplitude      */
  int   iztype;      /*    zero time equivalence  */
  int   unused16;    /*    reserved for future use*/
  int   iinst;       /*    recording instrument   */
  int   istreg;      /*    stn geographic region  */
  int   ievreg;      /*    event geographic region*/
  int   ievtyp;      /*    event type             */
  int   iqual;       /*    quality of data        */
  int   isynth;      /*    synthetic data flag    */
  int   imagtyp;     /*    reserved for future use*/
  int   imagsrc;     /*    reserved for future use*/
/* used for local coordinate identity
  int   unused19;    \*    reserved for future use*\
  int   icoor;       \*    type of local cartesian coordinate *\
  *tao: I decide not to used this simply because I don't know where it will be used.
*/
  int   unused19;    /*    reserved for future use*/
/*tao: The following is used for epoch time (struct timeval in time.h)
 *
  int   unused20;    \*    reserved for future use*\
  int   unused21;    \*    reserved for future use*\
  int   unused22;    \*    reserved for future use*\
  int   unused23;    \*    reserved for future use*\
 *
 * NOITCE: 
 *  The position here is designed to avoid data structure padding 
 *  (64bit x86: 8 byte).
 *
 *  There are (70 float + 28 int)=98*4=49*8 bytes before the two (long=int8) 
 *    values in struct timeval and after that there are (8 int)=8*4 bytes 
 *    before first occurrence of char[8]. 
 *
 *  So there is no padding. Thus keep sizeof(sac) equal the sum of size of
 *    all the members.
 *
 *  Sometimes the data structure padding may cause problem, such as reading
 *    a struct variable from a binary file using the sizeof(struct s).
 *    If there is padding, then sizeof(struct) would be larger than what is 
 *    really needed, and cause loss of data reading the left of the file.
 *
 *  To avoid this error, define the actual struct size in a macro, such as
 *    #define SAC_HEADER_SIZE 632 
 *    and use this macro while reading header from files.
 *
*/
  struct timeval epoch; /* Epoch time in high precision format: 
                            {sec, usec(1e-6)} */
  /*
  long  b_sec;       \*    Epoch time of the first sample (second)  *\
  long  b_usec;      \*    fraction part of Epoch time (microsecond)*\
  */
  int   unused24;    /*    reserved for future use*/
  int   unused25;    /*    reserved for future use*/
  int   unused26;    /*    reserved for future use*/
  int   leven;       /* RA data-evenly-spaced flag*/
  int   lpspol;      /*    station polarity flag  */
  int   lovrok;      /*    overwrite permission   */
  int   lcalda;      /*    calc distance, azimuth */
  int   unused27;    /*    reserved for future use*/
  char  kstnm[8];    /*  F station name           */
  char  kevnm[16];   /*    event name             */
  char  khole[8];    /*    man-made event name    */
  char  ko[8];       /*    event origin time id   */
  char  ka[8];       /*    1st arrival time ident */
  char  kt0[8];      /*    time pick 0 ident      */
  char  kt1[8];      /*    time pick 1 ident      */
  char  kt2[8];      /*    time pick 2 ident      */
  char  kt3[8];      /*    time pick 3 ident      */
  char  kt4[8];      /*    time pick 4 ident      */
  char  kt5[8];      /*    time pick 5 ident      */
  char  kt6[8];      /*    time pick 6 ident      */
  char  kt7[8];      /*    time pick 7 ident      */
  char  kt8[8];      /*    time pick 8 ident      */
  char  kt9[8];      /*    time pick 9 ident      */
  char  kf[8];       /*    end of event ident     */
  char  kuser0[8];   /*    available to user      */
  char  kuser1[8];   /*    available to user      */
  char  kuser2[8];   /*    available to user      */
  char  kcmpnm[8];   /*  F component name         */
  char  knetwk[8];   /*    network name           */
  char  kdatrd[8];   /*    date data read         */
  char  kinst[8];    /*    instrument name        */
  /* additional field */
  float *data;       /* KT pointer to data array  */
} sac;


/* size of sac header */
/*
sizeof(float)=4*byte=4*8*bits=32*bits
sizeof(int)=4*byte=32*bits
sizeof(char)=1*byte
sizeof(float *)=8*byte (x86_64)
*/
#define SAC_HEADER_SIZE 632 /* 70*float + 36*int + 2*long + 24*char[8] 
                             * not including the last (float *)data
                             */

/* index of sac header */
#define SAC_HEADER_FIELDS 133 /* 133 header fields */

#define SAC_HEADER_FLOAT_MIN 0
#define SAC_HEADER_FLOAT_MAX 69 /* 70 float4 */

#define SAC_HEADER_INT_MIN 70
#define SAC_HEADER_INT_MAX 84

#define SAC_HEADER_ENUM_MIN 85
#define SAC_HEADER_ENUM_MAX 104

#define SAC_HEADER_LOGICAL_MIN 105
#define SAC_HEADER_LOGICAL_MAX 109 /* 40 int4 */

#define SAC_HEADER_CHAR8_MIN 110
#define SAC_HEADER_CHAR16 111 /* this is kevnm[16] */
#define SAC_HEADER_CHAR8_MAX 132 /* 24 char[8] */


/* SAC header info 
 *
 * This is from Niu's seis/inc/sac.h
 */
#define EMPTY  0
#define FLT32  1
#define INT32  2
#define INT64  3
#define CHAR8  4
#define CHAR16 5

struct _SACHD_ {
  char  hdnm[16];
  int   pos;       /* position in the file */
  int   size;      /* byte size */ 
  int   type;      /* float=1 int=2 ...  */
};

extern struct _SACHD_ SACHD[SAC_HEADER_FIELDS]; /* defined in lib/sac_glob_lib.c */

#endif /* SAC_H */
