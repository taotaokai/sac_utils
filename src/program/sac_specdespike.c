/* Remove spikes in by sliding 1-D medians. */
/*
  Copyright (C) 2004 University of Texas at Austin
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>

#include "dbg.h"
#include "sac.h"
#include "sac_utils.h"
#include "getpar.h"
#include "seis.h"

/*=== self documentation ===*/ 
char *sdoc[] = {
"NAME ",
"  sacspecdespike ",
"   ",
"DESCRIPTION ",
"  Remove spectral spikes by sliding 1-D medians",
" ",
"SYNOPSIS ",
"  sacspecdespike in= out=despike.out width=1.0 ",
" ",
"COMMENTS ",
" ",
"PARAMETERS ",
"  string in=  input sac file name",
"  string out=despike.out  output sac file name",
"  float width=1.0  width of sliding spectral window (Hz)",
" ",
NULL};
/*=== END of self-doc ===*/ 


int main(int argc, char *argv[])
{
  char *in=NULL;
  char *out=NULL;
  float width;
 
  sac *tr=NULL;
  
  int i, n, nfft, nwin;
  double *x=NULL, *Ax=NULL, *Ay=NULL, *Ph=NULL;
  fftw_complex *Fx=NULL;
  fftw_plan pfft=NULL, pbfft=NULL;

  float *x_flt=NULL;

  /*=== Get parameters ===*/
  initargs(argc, argv);
  requestdoc(1);
  if (!getparstring("in", &in)) sentinel("No in= ");
  if (!getparstring("out", &out)) out="despike.out";
  if (!getparfloat("width",&width)) width=1.0;

  check(width>0, "width= must be a positive number");

  /*=== Load data ===*/
  check(sacchk(in)==0, "Not sac file: %s", in);
  tr = calloc(sizeof(sac),1);
  check(sacread(tr, in)==0, "Error read in sac file: %s", in);

  n = tr->npts;
  nwin = width*n*tr->delta;
  check(nwin>=1, "width= too small");
  
  debug("nwin=%d",nwin);

  /*=== Prepare FFTW ===*/
  nfft = n/2+1;
  x = malloc(sizeof(double)*n);
	Fx = fftw_malloc(sizeof(fftw_complex)*nfft);

	pfft = fftw_plan_dft_r2c_1d(n, x, Fx, FFTW_ESTIMATE); /* Forward */
	pbfft = fftw_plan_dft_c2r_1d(n, Fx, x, FFTW_ESTIMATE); /* Backward */

  /*=== Compute amplitude spectrum ===*/
  for (i=0; i<n; i++) x[i]=tr->data[i];
  fftw_execute(pfft);

  Ax = malloc(sizeof(double)*nfft);
  Ph = malloc(sizeof(double)*nfft);
  for (i=0; i<nfft; i++) {
    Ax[i]=cabs(Fx[i]);
    Ph[i]=carg(Fx[i]);
  }

  /*=== Despike amplitude spectrum ===*/
  Ay = malloc(sizeof(double)*nfft);
  despike(Ax, Ay, nfft, nwin);
  for (i=0; i<nfft; i++) Fx[i] = Ay[i]*cexp(I*Ph[i])/n;

  /*=== Backward FFT ===*/
  fftw_execute(pbfft);

  /*=== Write out SAC ===*/
  x_flt = malloc(sizeof(double)*n);
  for (i=0; i<n; i++) x_flt[i] = x[i];
  check(sacsetdata(tr, x_flt, n)==0, "Error set sac data");
  check(sacwrite(tr, out)==0, "Error write sac file");

  /*=== Clean up ===*/
  sacfreen(tr,1);
  free(x);
  free(Ax);
  free(Ay);
  free(Ph);
  fftw_free(Fx);
  fftw_destroy_plan(pfft);
  fftw_destroy_plan(pbfft);
  free(x_flt);
  return 0;

error:
  sacfreen(tr,1);
  free(x);
  free(Ax);
  free(Ay);
  free(Ph);
  fftw_free(Fx);
  fftw_destroy_plan(pfft);
  fftw_destroy_plan(pbfft);
  free(x_flt);
  return -1;
}

/*   $Id: Mdespike.c 11860 2014-02-22 16:38:50Z sfomel $   */
