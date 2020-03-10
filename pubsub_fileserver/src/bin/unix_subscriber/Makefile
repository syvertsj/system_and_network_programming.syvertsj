# syvertsj

PROJECT_ROOT = ../../..
LIBDIR = $(PROJECT_ROOT)/src/lib
INCDIR = $(PROJECT_ROOT)/include

# installation directory:
INSTDIR = $(PROJECT_ROOT)/bin

SRCS = unix_subscriber.c
LIBS = $(LIBDIR)/unixnitslib/unixnitslib.a

OBJS = $(SRCS:.c=.o)
PROJECT = unix_subscriber
FLAGS = -I$(INCDIR)
CC = g++ -g $(FLAGS)
test `uname` = "SunOS" && LFLAGS = "-lsocket -lnsl" 

#Suffix Rule
.c.o:
	@$(CC) -c $<

$(PROJECT): $(OBJS)
	@$(CC) $(OBJS) $(LIBS) -o $@ $(LFLAGS)

it: $(PROJECT) done

clean:
	@rm -f $(OBJS) $(PROJECT)
	@rm -rf $(INSTDIR)
	@sed -e '1,/^# DO NOT DELETE/!d' Makefile > make.tmp
	@mv make.tmp Makefile


depend: $(SRCS)
	@$(CC) -M $(SRCS) > dependList
	@sed -e '1,/^# DO NOT DELETE/!d' Makefile > make.tmp
	@cat dependList >> make.tmp
	@mv make.tmp Makefile
	@rm dependList

install: it 
	@install -d $(INSTDIR)
	@chmod 755 $(PROJECT)
	@cp -p $(PROJECT) $(INSTDIR)
	@echo
	@echo " -- $(PROJECT) installed --"
	@echo

done: 
	@echo
	@echo " -- $(PROJECT) built --"
	@echo

# DO NOT DELETE THIS LINE
