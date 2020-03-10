# syvertsj

PROJECT_ROOT = ../../..
LIBDIR = $(PROJECT_ROOT)/src/lib/unixnitslib
INCLDIR = $(PROJECT_ROOT)/include

# installation directory:
INSTDIR = $(PROJECT_ROOT)/lib

SRCS = unixnitslib.c
OBJS = $(SRCS:.c=.o)
PROJECT = unixnitslib.a
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
	@echo " -- $(PROJECT) installed -- "
	@echo

done: 
	@echo
	@echo " -- $(PROJECT) built --"
	@echo


# DO NOT DELETE THIS LINE
