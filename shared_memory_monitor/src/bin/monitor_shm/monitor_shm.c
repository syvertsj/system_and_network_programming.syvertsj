/**
 * monitor_shm.c
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


int main(int argc, char *argv[])
{
	int shmid;
	int duration = 30; 
	extern int Index;

	/* Create log file using log_mgr lib */
	set_logfile("monitor_shm.log");
	log_event(INFO, " -- Running %s with pid: [%d]\n", argv[0], getpid() );

	/* Check optional argument */
	if ( NULL != argv[1] ) { 
		duration = atoi( argv[1] );
		// verify argument is an integer
		log_event(INFO, " -- Running with set duration of %d seconds\n", duration);
	} else {
		log_event(INFO, " -- Running with default duration of %d seconds\n", duration);
	}

	/* Get table index */
	Index = get_index(KEY);

	/* Connect to shared memory segment */
	Shm_table[Index].Shm_area = (struct data *) connect_shm(KEY, DATASIZE);

	printf("Shm_area - 0x%x\n", Shm_table[Index].Shm_area);
	get_shm_info(Shm_table[Index].shmid);

	int i, j;
	float ave_x, ave_y;
	int tot_active; 
	for ( i = 1; i <= duration; i++ ) { 
		sleep(1);
		ave_x = 0;
		ave_y = 0;
		tot_active = 0;
		for (j = 0; j < DATASIZE; j++ ) {
			tot_active = tot_active + Shm_table[Index].Shm_area[j].is_valid;
			ave_x      = ave_x + Shm_table[Index].Shm_area[j].x;
			ave_y      = ave_y + Shm_table[Index].Shm_area[j].y;
		}
		ave_x = ave_x / tot_active;
		ave_y = ave_y / tot_active;

		fprintf(stderr, "\n %s: At time %d: %d active elements: x = %f and y = %f \n", argv[0], i, tot_active, ave_x, ave_y);
	}
	
	// detach shared memory segment
	if ( ERROR == detach_shm(Shm_table[Index].Shm_area) ) { 
		printf("\nError detaching shared memory\n");
	} else {
		printf("\nOK detaching shared memory\n");
	}

	exit(0);
}

