
.PHONY: all clean 

programs = uds_server uds_client tcp_server tcp_client

start:
	@echo "make [all|clean]"

all: libsock.so $(programs)

libsock.so: socklib.c
	@gcc -fPIC -c $<
	@gcc -shared -W socklib.o -o libsock.so.1
	@test -L libsock.so || ln -s libsock.so.1 libsock.so

uds_server: uds_server.c socklib.c
	gcc -g -o $@ $^

uds_client: uds_client.c socklib.c
	gcc -g -o $@ $^

tcp_server: tcp_server.c socklib.c
	gcc -g -o $@ $^

tcp_client: tcp_client.c socklib.c
	gcc -g -o $@ $^

clean:
	@rm -rf *.o libsock.* $(programs) *.dSYM
