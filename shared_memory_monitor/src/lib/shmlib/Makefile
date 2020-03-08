# syvertsj
# Makefile -- shmlib.a  makefile

PROJECT_ROOT = ../../..
LIBDIR = $(PROJECT_ROOT)/src/lib/shmlib
INCLDIR = $(PROJECT_ROOT)/include

# installation directory:
INSTDIR = $(PROJECT_ROOT)/lib

SRCS = shared_mem.c 
OBJS = $(SRCS:.c=.o)
PROJECT = shmlib.a
FLAGS = -I$(INCLDIR)
CC = gcc $(FLAGS)

#Suffix Rule
.c.o:
	$(CC) -c $<
 	
$(PROJECT): $(OBJS)
	ar -cr $(PROJECT) $(OBJS)

it: $(PROJECT) done

install: $(LIBDIR)/$(PROJECT)
$(LIBDIR)/$(PROJECT): $(PROJECT)
	cp $(PROJECT) $(LIBDIR)	

clean:
	rm -f $(OBJS) $(PROJECT)
	rm -rf $(INSTDIR)

depend: $(SRCS)
	$(CC) -M $(SRCS) > dependList
	sed -e '1,/^# DO NOT DELETE/!d' Makefile > make.tmp
	cat dependList >> make.tmp
	mv make.tmp Makefile
	rm dependList

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
