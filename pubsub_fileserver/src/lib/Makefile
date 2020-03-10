# syvertsj

# libs to be built:
SUB = unixnitslib tcpnitslib

it install depend clean:
	@-X=`pwd`; \
	for i in ${SUB}; \
	do echo; echo "<<< $$i >>>"; echo; cd $$X/$$i; make $@; done
