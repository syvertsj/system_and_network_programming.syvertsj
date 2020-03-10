# syvertsj 

SUB = src 

start: 
	@echo "[it|install|depend|clean|distribution|release]"

it install depend clean:
	@-X=`pwd`; \
	for i in ${SUB}; \
	do echo; echo "<<< $$i >>>"; echo; cd $$X/$$i; make $@; done
	@test $@ = clean && test -e *.tar && rm *.tar 2>/dev/null 

author = syvertsj
proj   = $$(basename `pwd`)

# archive source
distribution: clean
	@tar -C .. -cf $(author).$(proj).src.tar --exclude $(author).$(proj).src.tar $(proj)

# archive binaries
release: install
	@tar -C .. -cf $(author).$(proj).bin.tar $(proj)/bin $(proj)/lib

.PHONY: it install depend clean distribution release
