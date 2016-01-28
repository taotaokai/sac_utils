CC = gcc

RM = \rm -rf
MKDIR_P = \mkdir -p

obj_dir = obj
bin_dir = bin
inc_dir = include
src_dir = src
lib_dir = lib

libsrc_dir = ${src_dir}/libsrc
program_dir = ${src_dir}/program

# gcc
CCFLAGS = -g -Wall -Wextra -pedantic
#CCFLAGS = -O2
CCFLAGS += -I$(inc_dir)
LDFLAGS = -l SU_getpar

#lib = 
program = sac_dump

#------------------------------------------
module_obj = $(patsubst  %,$(obj_dir)/%.o, $(module))
shared_obj = $(patsubst  %,$(obj_dir)/%.o, $(shared))
program_obj = $(patsubst %,$(obj_dir)/%.o, $(program))

.PHONY: all clean directories

all : directories $(program)

directories:
	$(MKDIR_P) $(bin_dir) $(obj_dir)

$(shared_obj) :
	$(FC) -c $(shared_dir)/$(patsubst %.o,%.f90,$(@F)) -o $@ $(FCFLAGS)

$(module_obj) : $(shared_obj)
	$(FC) -c $(module_dir)/$(patsubst %.o,%.f90,$(@F)) -o $@ $(FCFLAGS)

$(program_obj) : $(shared_obj) $(module_obj)
	$(FC) -c $(program_dir)/$(patsubst %.o,%.f90,$(@F)) -o $@ $(FCFLAGS)

#xsem_slice_gcircle : $(program_obj) $(shared_obj) $(module_obj)
#	$(FC) -o $(bin_dir)/$@ $@ $(shared_obj) $(module_obj) $(FCFLAGS) $(LDFLAGS)

$(program) : $(program_obj) $(shared_obj) $(module_obj)
	$(FC) -o $(bin_dir)/$(@F) $(patsubst %,$(obj_dir)/%.o, $(@F)) \
		$(shared_obj) $(module_obj) $(FCFLAGS) $(LDFLAGS)

# explicit specified dependencies
#$(module): $(shared)
#$(OBJ): $(SHARED) $(MOD)
#$(SHARED) : $(ODIR)/constants_module.o


clean :
	$(RM)  $(bin_dir) $(obj_dir)
