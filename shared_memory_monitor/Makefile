########################################################
# Author : James A. Syvertsen
# Desc.  : calls makefile from lower level
########################################################

SUB = src 

start: 
	@echo "[it|install|depend|clean|distribution|release]"

it install depend clean:
	@-X=`pwd`; \
	for i in ${SUB}; \
	do echo; echo "<<< $$i >>>"; echo; cd $$X/$$i; make $@; done
	@test $@ = clean && test -e *.log && rm *.log 2>&-

.PHONY: it install depend clean 
