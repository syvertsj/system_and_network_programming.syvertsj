/**
 * install_and_monitor.c
 * 
 * James A. Syvertsen
 */ 

#include "common.h"
#include "log_mgr.h"

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


struct data {
	int is_valid;
	float x;
	float y;
};

struct data Shm_area[DATASIZE];

void *install_data(void *);
void *monitor_shm(void *);
void get_data(FILE *);
void init_table();
void *signal_handler(void *);



int main(int argc, char* argv[])
{
	pthread_t install_thread, monitor_thread, signal_thread;
	int rtn_val;
	int duration = 30;
	sigset_t    set;

	// install signal masking
	if ( 0 > sigfillset(&set) )
        perror("sigfillset error");

	if ( 0 > sigprocmask(SIG_SETMASK, &set, NULL) )
        perror("SIG_SETMASK error");

	// verify usage
	if ( argc < 2 || argc > 3) {
		printf("\nUsage:  %s <file> <monitor time>\n\n", argv[0]);
		exit(1);
	}

	// initialize table elements to zero
	init_table();

	// create signal handler thread
	if ( 0 != (rtn_val = pthread_create(&signal_thread, NULL, 
			signal_handler, (void*) argv[1])) ) 
	{
		fprintf (stderr, "pthread_create signal handler thread: error %d (%d)\n", rtn_val, errno);
	}

	// create install thread
	if ( 0 != (rtn_val = pthread_create(&install_thread, NULL,
		    install_data, (void*) argv[1])) )
	{
		fprintf (stderr, "pthread_create install thread: error %d (%d)\n", rtn_val, errno);
	}

	// read monitor time period 
	if ( NULL != argv[2] ) duration = atoi(argv[2]) ; 

	// create monitor thread
	if ( 0 != (rtn_val = pthread_create(&monitor_thread, NULL, 
		    monitor_shm, (void*) duration)) )
	{
		fprintf (stderr, "pthread_create monitor thread: error %d (%d)\n", rtn_val, errno);
	}

	// wait until threads terminate
	pthread_join(install_thread, NULL);
	pthread_join(monitor_thread, NULL);

	exit(0);
}

void *install_data(void *arg)
{
	pid_t     pid = getpid();
	pthread_t tid = pthread_self();
	FILE *data_file;

	fprintf(stderr, "\n pid: %u, INSTALL tid: %u ", pid, tid);

	// check file argument
	if ( NULL == ( data_file = fopen(arg, "r")) ) {
		perror("\n file-open error! \n"); 
		exit(1);
	}

	// parse data file and load into shared memory 
	get_data(data_file);

	// close data file
	fclose(data_file);

	pthread_exit(NULL);
}

void *monitor_shm(void *arg)
{
	pid_t     pid = getpid();
	pthread_t tid = pthread_self();
	int *seconds = (int *) arg;

	fprintf(stderr, "\n pid: %u, MONITOR tid: %u for %d seconds\n", pid, tid, seconds);

	int i, j;
	float ave_x, ave_y;
	int tot_active; 
	for ( i = 0; i <= (int)seconds; i++ ) {
		sleep(1);
		ave_x = 0.0;
		ave_y = 0.0;
		tot_active = 0;
		for (j = 0; j < DATASIZE; j++ ) {
			tot_active = tot_active + Shm_area[j].is_valid;
			ave_x      = ave_x + Shm_area[j].x;
			ave_y      = ave_y + Shm_area[j].y;
		}
		if ( 0 != ave_x ) 
			ave_x = ave_x / tot_active;
		if ( 0 != ave_y ) 
			ave_y = ave_y / tot_active;

		printf("\n monitor_data -- at time %d: %d active elements: x = %f and y = %f \n", i, tot_active, ave_x, ave_y);
	}

	pthread_exit(NULL);
}

void get_data(FILE *data_file)
{
	int lcount = 0;
	char line[80];
	int index = 0;
	int interval = 0;
	float x = 0.0;
	float y = 0.0;

	while ( NULL != (fgets(line, sizeof(line), data_file)) ) { 
		fprintf(stderr, "\n install_data -- read line %d: %s", lcount, line);
		index = atoi( (char *) strtok(line, " ") ); 
		x = atof( (char *) strtok(NULL, " ") ); 
		y = atof( (char *) strtok(NULL, " ") ); 
		interval = atoi( (char *) strtok(NULL, " ") ); 
		if ( interval >= DATASIZE ) {
			fprintf(stderr, "\nError in datafile: index exceeds shared memory table size");
		} else if ( interval >= 0 ) { 
			sleep(interval);
			Shm_area[index].is_valid = 1;
			Shm_area[index].x = x;
			Shm_area[index].y = y;
		} else { // ( interval < 0 )
			Shm_area[index].is_valid = 0;
			Shm_area[index].x = 0.0;
			Shm_area[index].y = 0.0;
		}
		lcount++;
	}
	return;
}

void init_table()
{
	int j;
	for (j = 0; j < DATASIZE; j++ ) {
		Shm_area[j].is_valid = 0;
		Shm_area[j].x = 0.0;
		Shm_area[j].y = 0.0;
	}

	return;
}

void *signal_handler(void *arg)
{
	sigset_t set;
	int signal;
	FILE *data_file;

	// install signal masking
	if ( 0 > sigfillset(&set) )
        perror("sigfillset error");

	if ( 0 > sigprocmask(SIG_SETMASK, &set, NULL) )
        perror("SIG_SETMASK error");

	// catch all signals 
	if ( 0 > sigfillset(&set) )
        perror("sigfillset error");

	while (1) {

 		// wait for a signal to arrive 
		if (0 != sigwait(&set, &signal) )
			perror("sigwait failed");

		switch (signal) {
		  case SIGTERM : 
				fprintf(stderr, "interrupted with signal: %d, exiting...\n", signal);
				exit(0); 
		  case SIGINT : 
				fprintf(stderr, "interrupted with signal: %d \n", signal);

				// clear table 
				init_table();
	
				// check file argument
				if ( NULL == ( data_file = fopen(arg, "r")) ) {
					perror("\n file-open error! \n"); 
					exit(1);
				} 

				// parse data file and load into shared memory 
				get_data(data_file);

				// close data file
				fclose(data_file);
		  default : printf("caught signal: %d\n", signal); 
		} 

	} 

	pthread_exit(NULL);
} 

