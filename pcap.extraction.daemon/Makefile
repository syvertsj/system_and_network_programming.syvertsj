
cc = g++ -std=gnu++11
flags = -DHAVE_64BIT_LONG_LONG

.PHONY: all install test clean 

start:
	@echo "make [all|install|clean]"

all: libsock.so extractd uds_extractor_client

libsock.so: socklib.c
	@g++ -fPIC -c $<
	@g++ -shared -Wl,-soname,libsock.so -o libsock.so.1 socklib.o
	@test -L libsock.so || ln -s libsock.so.1 libsock.so

uds_extractor_client: uds_extractor_client.c socklib.c
	gcc -g -o $@ $^

extractd: extractd.c socklib.c extractor.c
	gcc -g -o $@ $^ -lpcap

install: extractd init-extractd 
	sudo cp extractd /usr/sbin
	sudo cp extraction.data /tmp
	sudo cp init-extractd /etc/init.d/extractd
	sudo sysv-rc-conf --level 2345 extractd on

# note: "netcat" may be "nc" on different systems
test:
	@service extractd start
	@sleep 2
	@echo -n unix | netcat localhost 2112 &
	@sleep 2
	@./uds_extractor_client /tmp/req_unix
	@sleep 2
	@rm -f /tmp/req_unix 
	@sleep 2
	@service extractd stop

clean:
	@rm -f *.o libsock.* extractd uds_extractor_client 
