/**
 * shared_mem.h 
 * 
 * James A. Syvertsen
 */ 

#ifndef SHARED_MEM_H
#define SHARED_MEM_H

#define OK 0
#define ERROR -1

#define MAX_ATTACHMENTS	4096	


// data element
struct data {
	int is_valid;
	float x;
	float y;
};

// attachment element
struct shmat_element {
	int key;
	int shmid;
	struct data *Shm_area;
};

// attachment table
struct shmat_element Shm_table[MAX_ATTACHMENTS];

// index for attachment table
int Index;

/**
 * function:    get_key
 * param:       shared memory key 
 * return:      index for attachment table
 * description: just a modulus 'hash' for unique index given a key
 *
 */
int get_index(int key);

/**
 * function:    get_shm_info -- (not a requirement but informative)
 *
 */
void get_shm_info(int shmid);

/**
 * function:    connect_shm
 * param:       shared memory key 
 * param:       size in bytes of shared memory segment to be allocated
 * return:      address of shared memory segment
 * description: allocate and attach shared memory segment using given key
 */
void *connect_shm(int key, int size);

/**
 * function:    detach_shm
 * param:       address of shared memory segment
 * return:      OK/ERROR 
 * description: detach shared memory segment attached to process 
 */
int detach_shm(void *addr);

/**
 * function:    destroy_shm
 * param:       shared memory ID
 * return:      OK/ERROR 
 * description: delete shared memory segments with given key
 */
int destroy_shm(int key);


#endif /* SHARED_MEM_H */
