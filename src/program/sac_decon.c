/* SACDECON: $Revision: 0.0 $; $Date: 2014/08/31 09:39:00 $ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dbg.h"
#include "sac.h"
#include "sacutil.h"
#include "par.h"
#include "seis.h"

/*********************** self documentation ******************************/
char *sdoc[] = {
"NAME  ",
"  sacdecon ",
" ",
"DESCRIPTION ",
"  deconvolution with user-supplied filter by straightforward",
"  complex Division in the frequency domain with regularization",
"  ",
"SYNOPSIS ",
"  sacdecon in= filter= out=decon.out k=0.001 o=0 b=-inf e=inf",
" ",
"COMMENTS ",
"  For given time-domain input data D(t) and deconvolution  ",
"  filter F(t), the frequency-domain deconvolved trace can be written as:",
"                  ",
"         D(f)              D(f) * complex_conjugate[F(f)]     ",
"  U(f) = ---- ===> U(f) = --------------------------------    ",
"         F(f)             max(|F(f)|^2 , k*mean(|F(f)|^2))    ",
"                  ",
"  The real scalar k is introduced to prevent the resulting deconvolved",
"  trace to be dominated by frequencies at which the filter power is close",
"  to zero. ",
" ",
"  The filter file is windowed according to o/b/e and the zero time is set to ",
"  the time label specified by o= ",
" ",
"PARAMETERS ",
"  string in=  sac file to be deconvolved",
"  string filter=  sac file as the filter to be deconvovle out from input file",
"  string output=decon.out  sac file for the deconvolved result",
"  float k=0.001   water level for stabilizing results ",
"  string o=0  reference(zero) time label of filter file",
"              any float type header value (e.g. b, t0 etc), ",
"              or 0 (sac reference time)",
"  float b=-inf   window begin time, reference to o",
"  float e=inf    window end time, reference to o",
"  ",
"REFERENCES ",
"  SU: sucddecon.c         ",
"  Numerical Recipies in C: convolv.c         ",
" ",
NULL};
/**************** end self doc *******************************************/


int main(int argc, char **argv)
{
  char *in=NULL;
  char *filter=NULL;
  char *out=NULL;

  float o,b,e,k;
  char *otime;

  sac *sacin=sacnewn(1);
  sac *sacfilter=sacnewn(1);

  float diff;
  int i, npts_in, npts_filter, npts_out;
  double *xin=NULL, *xfilter=NULL, *xout=NULL;
  float *xout_float=NULL;

  /*=== Get parameters ===*/
  initargs(argc, argv);
  requestdoc(1);

  if (!getparstring("in", &in)) sentinel("No in="); 
  if (!getparstring("filter", &filter)) sentinel("No filter="); 
  if (!getparstring("out", &out)) out="decon.out";
  if (!getparstring("o", &otime)) otime="0";
  if (!getparfloat("b", &b)) b=-INFINITY;
  if (!getparfloat("e", &e)) e=INFINITY;
  if (!getparfloat("k", &k)) k=0.001;

  check(k>0, "k= must >0");

  /*=== Load data & Preprocess ===*/
  check(sacchk(in)==0, "in= not sac file");
  check(sacread(sacin, in)==0,
        "Error read input file: %s",in);

  check(sacchk(filter)==0, "filter= not sac file");
  check(sacread(sacfilter, filter)==0,
        "Error read filter file: %s",filter);

  /* make sure the same sampling interval */
  diff = fabs(1.0/sacin->delta - 1.0/sacfilter->delta);
  check(diff<1e-6,"Different sampling frequencies");

  /* get the reference time of the filter file */
  if (strcmp(otime,"0")==0)  o=0.0;
  else {
    o = sacgetfhv(sacfilter,otime);
    check(!isnan(o), "Invalid header name")
  }
  debug("filter reference time: %s=%f",otime,o);

  /* time window cut of filter file */
  check(saccut(sacfilter,b+o,e+o)==0,
        "Failed to window the filter");

  /* pad zeros */
  npts_in = sacin->npts;
  npts_filter = sacfilter->npts;
  npts_out = npts_in + npts_filter -1;

  xin = malloc(npts_out*sizeof(double));
  check_mem(xin);
  for(i=0; i<npts_filter-1; i++) xin[i] = 0.0;
  for(i=npts_filter-1; i<npts_out; i++) {
    xin[i] = sacin->data[i-npts_filter+1];
  }

  xfilter = malloc(npts_out*sizeof(double));
  check_mem(xfilter);
  for(i=0; i<npts_filter; i++) xfilter[i] = sacfilter->data[i];
  for(i=npts_filter; i<npts_out; i++) xfilter[i] = 0.0;

  /*=== deconvolution ===*/
  xout = malloc(npts_out*sizeof(double));
  check_mem(xout);

  deconv(xin, xfilter, xout, npts_out, k);

  xout_float = malloc(npts_out*sizeof(float));
  check_mem(xout_float);
  for(i=0; i<npts_out; i++) xout_float[i] = xout[i];

  /* modify input as output, and output result */
  debug("in->b=%f",sacin->b);
  sacin->b -= sacfilter->e - o;
  debug("out->b=%f",sacin->b);
  sacin->npts = npts_out;
  sacsetdata(sacin,xout_float,npts_out);
  sacwrite(sacin, out);

  /* free memory */
  free(xin);
  free(xfilter);
  free(xout);
  free(xout_float);
  sacfreen(sacin,1);
  sacfreen(sacfilter,1);
  return 0;

error:
  free(xin);
  free(xfilter);
  free(xout);
  free(xout_float);
  sacfreen(sacin,1);
  sacfreen(sacfilter,1);
  return -1;
}
