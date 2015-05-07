#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>

#include "dbg.h"
#include "sac.h"

/*********************** self documentation ******************************/
char *sdoc[] = {
"NAME  ",
"  saccorr ",
" ",
"DESCRIPTION ",
"  cross-correlation with of sliding window normalization  ",
" ",
"SYNOPSIS ",
"  saccorr in= filter= out=corr.out norm=y o=0 b=-inf e=inf ",
" ",
"COMMENTS ",
" ",
"  |--filter--|  >>> sliding and normalized correlation >>>  |--filter--| ",
"             |--------------------in------------------------| ",
"   ",
"  n1=length(filter), n2=length(in); ",
" ",
"  the cross-correlation result has index range from -(n1-1) to (n2-1),  ",
"  n1+n2-1 points in total. ",
" ",
"ALGORITHM (with fft) ",
" ",
"  |--filter--|0000000000000000000000000000 ",
"                >>>>>>>>>>>>>  |--filter--|0000000000000000000000000000 ",
"  |------------in-------------|00000000000|------------in-------------| ",
" ",
"  To avoid wrap around,  ",
"  in and filter are padded with zeros to the same length (n1+n2-1); ",
" ",
"  F(out) = F(in).*conj(F(filter)) ",
" ",
"  To normalize the cross-correlation,  ",
" ",
"  out[i] /= norm(filter)*mwnorm(in)[i] ",
" ",
"  , in which ",
" ",
"  mwnorm(in)[] = sqrt(abs(ifft(fft(in[].^2).*conj(fft(win[])))))  ",
"  is the moving window l2 norm of in[]) ",
"  win = 1 (i<n1); 0 (i>=n1); (the window is the same length as filter) ",
" ",
"  and norm(filter) = sqrt(sum(filter[].^2)) is the l2 norm of the filter; ",
" ",
"  To avoid numerical instability when dividing a very small number during  ",
"  normalization, a small positive value is added to the sliding norm of in[] ",
" ",
"PARAMETERS ",
"  string in=  sac file ",
"  string filter=  sac file to be cross-correlated with in= ",
"  string out=corr.out  output sac file of the cross-correlation result ",
"
"  string norm=y [y/n]  flag for sliding window normalization ",
"  float k=0.001  normalization coeff. only effective when norm=y ",
"
"  The following parameters specfify a time window on filter=
"  string o=0  reference(zero) time label of filter file ",
"              any float type header value (e.g. b, t0 etc) ",
"              or 0 (sac reference time) ",
"  float b=-inf   window begin time, reference to o ",
"  float e=inf    window end time, reference to o ",
" ",
NULL};
/**************** end self doc *******************************************/


int main(int argc, char **argv)
{
	char *in=NULL;
	char *filter=NULL;
	char *out=NULL;

	sac *sacin=sacnewn(1);
	sac *sacfilter=sacnewn(1);

	float diff;
	int i, npts_in, npts_filter;

  double *xin=NULL, *xfilter=NULL;

  /*=== Get parameters ===*/
  initargs(argc, argv);
  requestdoc(1);

  if (!getparstring("in", &in)) sentinel("No in="); 
  if (!getparstring("filter", &filter)) sentinel("No filter="); 
  if (!getparstring("out", &out)) out="corr.out";

  if (!getparstring("o", &otime)) otime="0";
  if (!getparfloat("b", &b)) b=-INFINITY;
  if (!getparfloat("e", &e)) e=INFINITY;

  if (!getparstring("norm", &norm)) norm="y";
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

	/*** normalize x1,x2 ***/
	rms_x1 = 0.0;
	for(i=0; i<n1; i++) rms_x1 += x1[i]*x1[i];
	rms_x1 = sqrt(rms_x1/n1);

	rms_x2 = 0.0;
	for(i=0; i<n2; i++) rms_x2 += x2[i]*x2[i];
	rms_x2 = sqrt(rms_x2/n2);

	for(i=0; i<n1; i++) x1[i] = x1[i]/rms_x1;
	for(i=0; i<n2; i++) x2[i] = x2[i]/rms_x2;

	log_info("RMS: master file %f data file %f",rms_x1,rms_x2);	

	/*** define FFTW ***/
	n4 = n3/2+1;
	Fx = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n4);
	Fy = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n4);

	x = (double *)malloc(sizeof(double)*n3);
	p1 = fftw_plan_dft_r2c_1d(n3, x, Fx, FFTW_ESTIMATE);
	p2 = fftw_plan_dft_c2r_1d(n3, Fx, x, FFTW_ESTIMATE);

	/*** sliding window norm of x2 (data file) ***/
	// x = x2.^2
	for(i=0; i<n2; i++) x[i] = x2[i]*x2[i]; //for stationary signal: amplitude around 1
	for(i=n2; i<n3; i++) x[i] = 0.;
	// Fy = fft(x2.^2)
	fftw_execute(p1);
	for(i=0; i<n4; i++) Fy[i]=Fx[i];
	// Fx = fft(win)
	for(i=0; i<n1; i++) x[i] = 1.; //should = 1/n1 here, moved below in sqrt(fabs(x[i]/n1/n3))
	for(i=n1; i<n3; i++) x[i] = 0.;
	fftw_execute(p1);
	// swnorm_x2 = sqrt(abs(ifft(fft(x2.^2).*conj(fft(win)))))
	for(i=0; i<n4; i++) Fx[i] = Fy[i]*conj(Fx[i]);
	fftw_execute(p2);
	swnorm_x2 = (double *)malloc(sizeof(double)*n3);
	for(i=0; i<n3; i++) swnorm_x2[i] = sqrt(fabs(x[i]/n1/n3)); //for stationary signal: amplitude around 1

	log_info("DONE: sliding window L2 norm of data file");

	/*** cross-correlation ***/
	// Fy = fft(x1)
	for(i=0; i<n3; i++) x[i]=x1[i];
	fftw_execute(p1);
	for(i=0; i<n4; i++) Fy[i]=Fx[i];
	// Fx = fft(x2)
	for(i=0; i<n3; i++) x[i]=x2[i];
	fftw_execute(p1);
	// Fx = fft(x2).*conj(fft(x1))
	for(i=0; i<n4; i++) Fx[i] = Fx[i]*conj(Fy[i]);
	// x = ifft(fft(x2).*conj(fft(x2)))
	fftw_execute(p2);
	// shift negative time for x, apply normalization coeff. for ifft (/n3), sliding window norm (/swnorm_x2)
	// x1 = x/n3/n1/swnorm_x2
	for(i=0; i<n1-1; i++) x1[i] = x[n2+i]/n3/n1/(swnorm_x2[n2+i] + lamda);
	for(i=n1-1; i<n3; i++) x1[i] = x[i+1-n1]/n3/n1/(swnorm_x2[i+1-n1] + lamda);

	log_info("DONE: cross-correlation");

	/*** save result ***/
	sac2->b = sac2->b - (sac2->delta)*(n1-1) - (sac1->b - mT0);
	sac2->npts = n3;
	sac_moddat(sac2,x1,n3);
	check(sac_w1sac(sac2,ccFile)==0,"Error write %s",ccFile);

	log_info("Write out results: %s",ccFile);

	/*** free memory ***/
	free(tofree);

	free(x1);
	free(x2);
	free(x);
	free(swnorm_x2);

	fftw_destroy_plan(p1);	
	fftw_destroy_plan(p2);	
	fftw_free(Fx); 
	fftw_free(Fy);

	sac_free(sac1,1);
	sac_free(sac2,1);
	
	return 0;

error:
	free(tofree);

	if (sac1) sac_free(sac1,1);	
	if (sac2) sac_free(sac2,1);	

	return -1;
}
