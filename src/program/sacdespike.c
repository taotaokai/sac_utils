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

#include "dbg.h"
#include "sac.h"
#include "sacutil.h"
#include "par.h"
#include "seis.h"

/*=== self documentation ===*/ 
char *sdoc[] = {
"NAME ",
"  sacdespike ",
"   ",
"DESCRIPTION ",
"  Remove spikes by sliding 1-D medians. ",
" ",
"SYNOPSIS ",
"  sacdespike in= out=despike.out width=1.0 ", 
" ",
"COMMENTS ",
" ",
"PARAMETERS ",
"  string in=  input sac file name",
"  string out=despike.out  output sac file name",
"  float width=1.0  width of sliding window",
" ",
NULL};
/*=== END of self-doc ===*/ 

/* IMAX, IMIN: Numerical Recipes in C */
static int imaxarg1,imaxarg2;
#define IMAX(a,b) (imaxarg1=(a),imaxarg2=(b),(imaxarg1) > (imaxarg2) ?\
        (imaxarg1) : (imaxarg2))

static int iminarg1,iminarg2;
#define IMIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ?\
        (iminarg1) : (iminarg2))


int main(int argc, char *argv[])
{
  char *in=NULL;
  char *out=NULL;
  float width;
  
  sac *tr=NULL;
  int status;

  int len, halflen, n1, i1, shift, i;
  float *newdat=NULL, *win=NULL;

  /*=== Get parameters ===*/
  initargs(argc, argv);
  requestdoc(1);
  if (!getparstring("in", &in)) {
    sentinel("No in= ");
  }
  if (!getparstring("out", &out)) out="despike.out";
  if (!getparfloat("width",&width)) width=1.0;

  /*=== Load data ===*/
  tr = malloc(sizeof(sac));
  status = sacread(tr, in);
  check(status==0, "Error read in sac file: %s", in);

  /*=== Despike ===*/
  n1 = tr->npts;
  len = width/tr->delta;
  halflen = len/2;

  newdat = malloc(n1*sizeof(float));
  win = malloc(len*sizeof(float));

  for (i1=0; i1 < n1; i1++) {
    shift = IMAX(0, IMIN(n1-len, i1-halflen));
    for (i=0; i<len; i++) win[i] = tr->data[shift+i];
    newdat[i1] = sf_quantile(halflen,len,win);
  }

  /*=== Outpt ===*/
  sacsetdata(tr, newdat, n1);
  sacwrite(tr, out);

  /*=== Clean up ===*/
  sacfreen(tr,1);
  free(newdat);
  free(win);

  return 0;

error:
  sacfreen(tr,1);
  free(newdat);
  free(win);
  return -1;
}

/*   $Id: Mdespike.c 11860 2014-02-22 16:38:50Z sfomel $   */
