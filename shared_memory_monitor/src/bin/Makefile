########################################################
# Author : James A. Syvertsen
# Desc.  : 'bin' Makefile
########################################################

SUB = install_data monitor_shm install_and_monitor

it install depend clean:
	@-X=`pwd`; \
	for i in ${SUB}; \
	do echo; echo "<<< $$i >>>"; echo; cd $$X/$$i; make $@; done
