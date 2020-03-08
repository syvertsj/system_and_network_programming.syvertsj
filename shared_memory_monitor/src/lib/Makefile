########################################################
# Author : James A. Syvertsen
# Desc.  : Calls the makefile from it lower level
########################################################

# libs to be built:
SUB = log_mgr shmlib

it install depend clean:
	@-X=`pwd`; \
	for i in ${SUB}; \
	do echo; echo "<<< $$i >>>"; echo; cd $$X/$$i; make $@; done
