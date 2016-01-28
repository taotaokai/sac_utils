#Makefile for program sac2xy

#Variables
#---------
#F90=g95
F90=gfortran
FFLAGS=-O2 -Wall -pedantic
RM=/bin/rm -f
BINDIR=./

all : main

#Compile modules.
mod_sac_io.o : mod_sac_io.f90
	$(F90) $(FFLAGS) -c mod_sac_io.f90

#Compile Source-code and link.
sac2xy : sac2xy.f90
	$(F90) $(FFLAGS) sac2xy.f90 -o sac2xy ./mod_sac_io.o

#Copy executable and man page to appropriate directories.
main : mod_sac_io.o sac2xy

clean :
	  $(RM) sac2xy mod_sac_io.o sac_i_o.mod
