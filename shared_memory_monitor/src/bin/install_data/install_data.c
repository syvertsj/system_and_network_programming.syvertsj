/**
 * install_data.c
 * 
 * James A. Syvertsen
 */ 

#include "common.h"
#include "shared_mem.h"
#include "log_mgr.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>

// shm attachment table index
extern int Index;

// define name of program outside scope of main (for logging)
char *Argv0; 

// input file
FILE *data_file;

void clear_shm();
void get_data();
static void signal_handler(int);


int main(int argc, char *argv[])
{
	int shmid;
	Argv0 = argv[0];

   /* Install signal handler */
   signal(SIGHUP, signal_handler); 
   signal(SIGTERM, signal_handler);

   // usage check 
   if ( 2 != argc ) {
		printf("\n Usage:  %s <file> \n\n", argv[0]); 
		exit(1);
	}

	// check file argument
	if ( NULL == (data_file = fopen(argv[1], "r")) ) {
		perror("\n file-open error! \n"); 
		log_event(FATAL, "%s with pid: [%d] -- error opening file: %s \n", argv[0], getpid(), argv[1] );
		exit(ERROR); 
	}

	// obtain index 
	if ( MAX_ATTACHMENTS < (Index = get_index(KEY) ) ) {
		fprintf(stderr, "\nError: shm attachment table greater than system max! Exiting...\n");
		exit(1);
	}

	// allocate and attach shared memory
	Shm_table[Index].Shm_area = (struct data *) connect_shm(KEY, DATASIZE); 

	// clear shared memory area
	clear_shm();

	// parse data file and load into shared memory 
	get_data();

	// close data file
	fclose(data_file);

	return 0;
}

void clear_shm()
{
	int i;
	for ( i = 0; i < DATASIZE; i++ ) { 
		Shm_table[Index].Shm_area[i].is_valid = 0;
		Shm_table[Index].Shm_area[i].x = 0.0;
		Shm_table[Index].Shm_area[i].y = 0.0;
	}

	return;
}

void get_data()
{
	extern char *Argv0;
	int lcount = 0;
	char line[80];
	int index = 0;
	int interval = 0;
	float x = 0.0;
	float y = 0.0;

	while ( NULL != (fgets(line, sizeof(line), data_file)) ) { 
		//printf("\n line %d: %s", lcount, line);
		index = atoi( (char *) strtok(line, " ") ); 
		x = atof( (char *) strtok(NULL, " ") ); 
		y = atof( (char *) strtok(NULL, " ") ); 
		interval = atoi( (char *) strtok(NULL, " ") ); 
		if ( interval >= DATASIZE ) {
			fprintf(stderr, "\nError in datafile: index exceeds shared memory table size");
		} else if ( interval >= 0 ) { 
			sleep(interval);
			Shm_table[Index].Shm_area[index].is_valid = 1;
			Shm_table[Index].Shm_area[index].x = x;
			Shm_table[Index].Shm_area[index].y = y;
			fprintf(stderr, "\n %s: for index: %d, is_valid: %d, x: %f, y: %f",  Argv0, index, \
				Shm_table[Index].Shm_area[index].is_valid, \
				Shm_table[Index].Shm_area[index].x, \
				Shm_table[Index].Shm_area[index].y);
		} else {
			Shm_table[Index].Shm_area[index].is_valid = 0;
			Shm_table[Index].Shm_area[index].x = 0.0;
			Shm_table[Index].Shm_area[index].y = 0.0;
			fprintf(stderr, "\n %s: for index: %d, is_valid: %d, x: %f, y: %f", Argv0, index, \
				Shm_table[Index].Shm_area[index].is_valid, \
				Shm_table[Index].Shm_area[index].x, \
				Shm_table[Index].Shm_area[index].y);
		}
		lcount++;
	}
	return;
}

static void signal_handler(int sig)
{
	extern char *Argv0;

   // Re-install signal handler 
   signal(SIGHUP, signal_handler); 
   signal(SIGTERM, signal_handler);

   if ( sig == SIGHUP ) {
      fprintf(stderr, "\n received SIGHUP ");
		log_event(WARNING, "%s with pid: [%d] -- received SIGHUP \n", Argv0, getpid() );

		// clear shared memory area
		clear_shm();

		// re-install data from beginning of data file
		get_data();	
   }
   if ( sig == SIGTERM ) {
      fprintf(stderr, "\n received SIGTERM");
		log_event(WARNING, "%s with pid: [%d] -- received SIGTERM\n", Argv0, getpid() );
	
		// detach shared memory segment
		if ( ERROR == detach_shm(Shm_table[Index].Shm_area) ) { 
			fprintf(stderr, "\nError detaching shared memory\n");
			log_event(WARNING, "%s with pid: [%d] -- Error detaching shared memory\n", Argv0, getpid() );
		} else {
			fprintf(stderr, "\nOK detaching shared memory\n");
		}

		// destroy shared memory segment
		if ( ERROR == destroy_shm(KEY) ) { 
			fprintf(stderr, "\nError destroying shared memory\n");
			log_event(WARNING, "%s with pid: [%d] -- Error destroying shared memory\n", Argv0, getpid() );
		} else {
			fprintf(stderr, "\nOK destroying shared memory\n");
		}
	
		fprintf(stderr, "\n%s exiting...\n", Argv0);

		close_logfile();

		exit(1);
   }
}

