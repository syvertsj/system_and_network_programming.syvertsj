# syvertsj

PROJECT_ROOT = ../../..
LIBDIR = $(PROJECT_ROOT)/src/lib/tcpnitslib
INCLDIR = $(PROJECT_ROOT)/include

# installation directory:
INSTDIR = $(PROJECT_ROOT)/lib

SRCS = tcpnitslib.c 
OBJS = $(SRCS:.c=.o)
PROJECT = tcpnitslib.a
FLAGS = -I$(INCLDIR)
CC = g++ -g $(FLAGS)

#Suffix Rule
.c.o:
	@$(CC) -c $<
 	
$(PROJECT): $(OBJS)
	@ar -cr $(PROJECT) $(OBJS)

it: $(PROJECT) done

install: $(LIBDIR)/$(PROJECT)
$(LIBDIR)/$(PROJECT): $(PROJECT)
	@cp $(PROJECT) $(LIBDIR)	

clean:
	@rm -f $(OBJS) $(PROJECT) 
	@rm -rf $(INSTDIR)

depend: $(SRCS)
	$(CC) -M $(SRCS) > dependList
	@sed -e '1,/^# DO NOT DELETE/!d' Makefile > make.tmp
	@cat dependList >> make.tmp
	@mv make.tmp Makefile
	@rm dependList

install: it 
	@install -d $(INSTDIR)
	@#install -c -m 755 $(PROJECT) $(INSTDIR)
	@chmod 755 $(PROJECT)
	@cp -p $(PROJECT) $(INSTDIR)
	@echo
	@echo " -- $(PROJECT) installed --"
	@echo

done: 
	@echo
	@echo " -- $(PROJECT) built -- "
	@echo


# DO NOT DELETE THIS LINE
