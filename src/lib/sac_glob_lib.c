#include "sac.h"

/* Definition of global variable
 *
 */

struct _SACHD_ SACHD[] = {
  { "delta",       0,  4,  FLT32 }, /* RF time increment, sec    */
  { "depmin",      4,  4,  FLT32 }, /*    minimum amplitude      */
  { "depmax",      8,  4,  FLT32 }, /*    maximum amplitude      */
  { "scale",      12,  4,  FLT32 }, /*    amplitude scale factor */
  { "odelta",     16,  4,  FLT32 }, /*    observed time inc      */
  { "b",          20,  4,  FLT32 }, /* RD initial value, time    */
  { "e",          24,  4,  FLT32 }, /* RD final value, time      */
  { "o",          28,  4,  FLT32 }, /*    event start, sec < nz. */
  { "a",          32,  4,  FLT32 }, /*    1st arrival time       */
  { "fmt",        36,  4,  FLT32 }, /*    internal use           */
  { "t0",         40,  4,  FLT32 }, /*    user-defined time pick */
  { "t1",         44,  4,  FLT32 }, /*    user-defined time pick */
  { "t2",         48,  4,  FLT32 }, /*    user-defined time pick */
  { "t3",         52,  4,  FLT32 }, /*    user-defined time pick */
  { "t4",         56,  4,  FLT32 }, /*    user-defined time pick */
  { "t5",         60,  4,  FLT32 }, /*    user-defined time pick */
  { "t6",         64,  4,  FLT32 }, /*    user-defined time pick */
  { "t7",         68,  4,  FLT32 }, /*    user-defined time pick */
  { "t8",         72,  4,  FLT32 }, /*    user-defined time pick */
  { "t9",         76,  4,  FLT32 }, /*    user-defined time pick */
  { "f",          80,  4,  FLT32 }, /*    event end, sec > nz    */
  { "resp0",      84,  4,  FLT32 }, /*    instrument respnse parm*/
  { "resp1",      88,  4,  FLT32 }, /*    instrument respnse parm*/
  { "resp2",      92,  4,  FLT32 }, /*    instrument respnse parm*/
  { "resp3",      96,  4,  FLT32 }, /*    instrument respnse parm*/
  { "resp4",     100,  4,  FLT32 }, /*    instrument respnse parm*/
  { "resp5",     104,  4,  FLT32 }, /*    instrument respnse parm*/
  { "resp6",     108,  4,  FLT32 }, /*    instrument respnse parm*/
  { "resp7",     112,  4,  FLT32 }, /*    instrument respnse parm*/
  { "resp8",     116,  4,  FLT32 }, /*    instrument respnse parm*/
  { "resp9",     120,  4,  FLT32 }, /*    instrument respnse parm*/
  { "stla",      124,  4,  FLT32 }, /*  T station latitude       */
  { "stlo",      128,  4,  FLT32 }, /*  T station longitude      */
  { "stel",      132,  4,  FLT32 }, /*  T station elevation, m   */
  { "stdp",      136,  4,  FLT32 }, /*  T station depth, m       */
  { "evla",      140,  4,  FLT32 }, /*    event latitude         */
  { "evlo",      144,  4,  FLT32 }, /*    event longitude        */
  { "evel",      148,  4,  FLT32 }, /*    event elevation        */
  { "evdp",      152,  4,  FLT32 }, /*    event depth            */
  { "mag",       156,  4,  FLT32 }, /*    reserved for future use*/
  { "user0",     160,  4,  FLT32 }, /*    available to user      */
  { "user1",     164,  4,  FLT32 }, /*    available to user      */
  { "user2",     168,  4,  FLT32 }, /*    available to user      */
  { "user3",     172,  4,  FLT32 }, /*    available to user      */
  { "user4",     176,  4,  FLT32 }, /*    available to user      */
  { "user5",     180,  4,  FLT32 }, /*    available to user      */
  { "user6",     184,  4,  FLT32 }, /*    available to user      */
  { "user7",     188,  4,  FLT32 }, /*    available to user      */
  { "user8",     192,  4,  FLT32 }, /*    available to user      */
  { "user9",     196,  4,  FLT32 }, /*    available to user      */
  { "dist",      200,  4,  FLT32 }, /*    stn-event distance, km */
  { "az",        204,  4,  FLT32 }, /*    event-stn azimuth      */
  { "baz",       208,  4,  FLT32 }, /*    stn-event azimuth      */
  { "gcarc",     212,  4,  FLT32 }, /*    stn-event dist, degrees*/
  { "sb",        216,  4,  FLT32 }, /*    internal use           */
  { "sdelta",    220,  4,  FLT32 }, /*    internal use           */
  { "depmen",    224,  4,  FLT32 }, /*    mean value, amplitude  */
  { "cmpaz",     228,  4,  FLT32 }, /*  T component azimuth      */
  { "cmpinc",    232,  4,  FLT32 }, /*  T component inclination  */
  { "xminimum",  236,  4,  FLT32 }, /*    reserved for future use*/
  { "xmaximum",  240,  4,  FLT32 }, /*    reserved for future use*/
  { "yminimum",  244,  4,  FLT32 }, /*    reserved for future use*/
  { "ymaximum",  248,  4,  FLT32 }, /*    reserved for future use*/
  { "stx",       252,  4,  FLT32 }, /* KT station x coordinate   */
  { "sty",       256,  4,  FLT32 }, /* KT station y coordinate   */
  { "stz",       260,  4,  FLT32 }, /* KT station z coordinate   */
  { "evx",       264,  4,  FLT32 }, /* KT event x coordinate     */
  { "evy",       268,  4,  FLT32 }, /* KT event y coordinate     */
  { "evz",       272,  4,  FLT32 }, /* KT event z coordinate     */
  { "unused12",  276,  4,  FLT32 }, /*    reserved for future use*/
  { "nzyear",    280,  4,  INT32 }, /*  F zero time of file, yr  */
  { "nzjday",    284,  4,  INT32 }, /*  F zero time of file, day */
  { "nzhour",    288,  4,  INT32 }, /*  F zero time of file, hr  */
  { "nzmin",     292,  4,  INT32 }, /*  F zero time of file, min */
  { "nzsec",     296,  4,  INT32 }, /*  F zero time of file, sec */
  { "nzmsec",    300,  4,  INT32 }, /*  F zero time of file, msec*/
  { "nvhdr",     304,  4,  INT32 }, /*    internal use           */
  { "norid",     308,  4,  INT32 }, /*    origin ID              */
  { "nevid",     312,  4,  INT32 }, /*    event ID               */
  { "npts",      316,  4,  INT32 }, /* RF number of samples      */
  { "nsnpts",    320,  4,  INT32 }, /*    internal use           */
  { "nwfid",     324,  4,  INT32 }, /*    waveform ID            */
  { "nxsize",    328,  4,  INT32 }, /*    reserved for future use*/
  { "nysize",    332,  4,  INT32 }, /*    reserved for future use*/
  { "nsps",      336,  4,  INT32 }, /* KT number of samples per second, sampling rate */
  { "iftype",    340,  4,  INT32 }, /* RA type of file           */
  { "idep",      344,  4,  INT32 }, /*    type of amplitude      */
  { "iztype",    348,  4,  INT32 }, /*    zero time equivalence  */
  { "unused16",  352,  4,  INT32 }, /*    reserved for future use*/
  { "iinst",     356,  4,  INT32 }, /*    recording instrument   */
  { "istreg",    360,  4,  INT32 }, /*    stn geographic region  */
  { "ievreg",    364,  4,  INT32 }, /*    event geographic region*/
  { "ievtyp",    368,  4,  INT32 }, /*    event type             */
  { "iqual",     372,  4,  INT32 }, /*    quality of data        */
  { "isynth",    376,  4,  INT32 }, /*    synthetic data flag    */
  { "imagtyp",   380,  4,  INT32 }, /*    reserved for future use*/
  { "imagsrc",   384,  4,  INT32 }, /*    reserved for future use*/
  { "unused19",  388,  4,  INT32 }, /*    reserved for future use*/
  { "epoch_sec", 392,  8,  INT64 }, /*    seconds of epoch time  */
  { "empty",     400,  0,  EMPTY }, /*    empty field            */
  { "epoch_usec",400,  8,  INT64 }, /* microseconds of epoch time*/
  { "empty",     408,  0,  EMPTY }, /*    empty field            */
  { "unused24",  408,  4,  INT32 }, /*    reserved for future use*/
  { "unused25",  412,  4,  INT32 }, /*    reserved for future use*/
  { "unused26",  416,  4,  INT32 }, /*    reserved for future use*/
  { "leven",     420,  4,  INT32 }, /* RA data-evenly-spaced flag*/
  { "lpspol",    424,  4,  INT32 }, /*    station polarity flag  */
  { "lovrok",    428,  4,  INT32 }, /*    overwrite permission   */
  { "lcalda",    432,  4,  INT32 }, /*    calc distance, azimuth */
  { "unused27",  436,  4,  INT32 }, /*    reserved for future use*/
  { "kstnm",     440,  8,  CHAR8 }, /*  F station name           */
  { "kevnm",     448, 16,  CHAR16}, /*    event name             */
  { "khole",     464,  8,  CHAR8 }, /*    man-made event name    */
  { "ko",        472,  8,  CHAR8 }, /*    event origin time id   */
  { "ka",        480,  8,  CHAR8 }, /*    1st arrival time ident */
  { "kt0",       488,  8,  CHAR8 }, /*    time pick 0 ident      */
  { "kt1",       496,  8,  CHAR8 }, /*    time pick 1 ident      */
  { "kt2",       504,  8,  CHAR8 }, /*    time pick 2 ident      */
  { "kt3",       512,  8,  CHAR8 }, /*    time pick 3 ident      */
  { "kt4",       520,  8,  CHAR8 }, /*    time pick 4 ident      */
  { "kt5",       528,  8,  CHAR8 }, /*    time pick 5 ident      */
  { "kt6",       536,  8,  CHAR8 }, /*    time pick 6 ident      */
  { "kt7",       544,  8,  CHAR8 }, /*    time pick 7 ident      */
  { "kt8",       552,  8,  CHAR8 }, /*    time pick 8 ident      */
  { "kt9",       560,  8,  CHAR8 }, /*    time pick 9 ident      */
  { "kf",        568,  8,  CHAR8 }, /*    end of event ident     */
  { "kuser0",    576,  8,  CHAR8 }, /*    available to user      */
  { "kuser1",    584,  8,  CHAR8 }, /*    available to user      */
  { "kuser2",    592,  8,  CHAR8 }, /*    available to user      */
  { "kcmpnm",    600,  8,  CHAR8 }, /*  F component name         */
  { "knetwk",    608,  8,  CHAR8 }, /*    network name           */
  { "kdatrd",    616,  8,  CHAR8 }, /*    date data read         */
  { "kinst",     624,  8,  CHAR8 }, /*    instrument name        */
}; 
/* END of SACHD */
