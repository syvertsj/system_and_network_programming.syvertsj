########################################################
# Author : James A. Syvertsen
# Desc.  : call makefile from lower level
########################################################

# need the libs built first:
SUB = lib bin

it install depend clean:
	@-X=`pwd`; \
	for i in ${SUB}; \
	do echo; echo "<<< $$i >>>"; echo; cd $$X/$$i; make $@; done
