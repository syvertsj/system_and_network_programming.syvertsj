# syvertsj 

SUB = unix_publisher unix_subscriber tcp_publisher tcp_subscriber

it install depend clean:
	@-X=`pwd`; \
	for i in ${SUB}; \
	do echo; echo "<<< $$i >>>"; echo; cd $$X/$$i; make $@; done
