/**
 * shared_mem.c
 * 
 * James A. Syvertsen
 */ 

#include "shared_mem.h"

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pwd.h>
#include <grp.h>


// private methods

void get_shm_info (int shmid)
{
        struct shmid_ds info;
        char *getname(ushort);
        char *getgroup(ushort);

        shmctl (shmid, IPC_STAT, &info);

        printf ("Permissions:\n");
        printf ("uid:   %s\n", getname (info.shm_perm.uid));
        printf ("gid:   %s\n", getgroup (info.shm_perm.gid));
        printf ("cuid:  %s\n", getname (info.shm_perm.cuid));
        printf ("cgid:  %s\n", getgroup (info.shm_perm.cgid));
        printf ("mode:  %o\n", info.shm_perm.mode);
        /*Not on ptux printf ("seq:   %d\n", info.shm_perm.seq);
        printf ("key:   %d\n", info.shm_perm.key);*/
}

char *getname(ushort uid)
{
        struct passwd *val;
        struct passwd *getpwuid();

        val = getpwuid (uid);

        return (val->pw_name);
}

char *getgroup(ushort gid)
{
        struct group *val;
        struct group *getgrgid();

        val = getgrgid (gid);

        return (val->gr_name);
}

// public methods

int get_index(int key)
{
	return key % 13;
}

void *connect_shm(int key, int size)
{
	/* Get key */
	Index = get_index(key);

	/* Store shared memory key */
	Shm_table[Index].key = key;

	/* Obtain shared memory identifier */
	Shm_table[Index].shmid = shmget(key, sizeof(struct data) * size, IPC_CREAT | 0666);

	if ( 0 > Shm_table[Index].shmid ) {
		perror ("shmget");
		return NULL;
	}

	/* Attach shared memory segment to address space */
	Shm_table[Index].Shm_area = (struct data *) shmat(Shm_table[Index].shmid, (void *) 0, 0); 

	printf("\nreturning shm address");
	return Shm_table[Index].Shm_area;
/*
	if ( 0 > (int) Shm_table[0].Shm_area ) {
		perror ("shmat");
		return NULL;
	} else { 
		// initialize all members of array to 0 
		printf("\nreturning shm address");
		return Shm_table[0].Shm_area;
	}
*/

}

int detach_shm(void *addr)
{
	/* Search shared memory table for matching address */
	for ( Index = 0; addr != Shm_table[Index].Shm_area; Index++ ) { ; }

	// no match
	if ( addr != Shm_table[Index].Shm_area ) { 
		printf("\n no matching address in shared memory table");
		return ERROR;
	}
	// matched, now detach
	if ( 0 > shmdt(Shm_table[Index].Shm_area ) ) { 
		perror("shmdt");
		return ERROR;
	} else { 
		return OK;
	}
}

int destroy_shm(int key)
{
	/* Get key */
	Index = get_index(key);

	/* Find match for key in table */
	printf("\n given key is: %d, stored key is: %d\n", key, Shm_table[Index].key);
	if ( key == Shm_table[Index].key) { 
		/* Delete shared memory segment from system */
		if ( 0 > shmctl(Shm_table[Index].shmid, IPC_RMID, NULL) ) { 
			perror("shmctl");
			return ERROR;
		} else { 
			return OK;
		}
	} else { 
		printf("\nNo matching key in shared memory table");
		return ERROR;
	}
}
