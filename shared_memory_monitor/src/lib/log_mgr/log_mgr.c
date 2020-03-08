/**
 * log_mgr.c
 * 
 * James A. Syvertsen
 */ 

#include "log_mgr.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// vsprintf(3):
#include <stdarg.h>

// time(2), ctime(3):
#include <time.h>

// open(2):
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// write(2), close(2):
#include <unistd.h>


/* global file descriptor */
int Global_fd = NULLFD;

/* global default log file name */
const char *Name_logfile = "logfile";


int log_event(Levels l, const char *fmt, ...)
{
	/* get time string for log */
	time_t logtime   = time(NULL);
	char *logstring  = ctime(&logtime);

	/* trim newline character */
	logstring[24] = 0; 

	/* assign debug text for enumerated log levels */
	const char *level;
	switch (l) {
		case INFO:    
			level = "INFO";  
			break; 
		case WARNING: 
			level = "WARNING";  
			break; 
		case FATAL:   
			level = "FATAL"; 
			break;
		default:      
			printf("\n log_event: unknown level: %s, used.\n", level); 
			return ERROR;
	}

	/* form variable argument buffer */
	char fmt_buffer[80];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(fmt_buffer, fmt, ap);
	va_end(ap); 

	/* form log string */
	strcat(logstring, ":"); 
	strcat(logstring, level); 
	strcat(logstring, ":"); 
	strcat(logstring, fmt_buffer);
	strcat(logstring, "\n");

	/* determine buffer size and copy to it */
	int strlength = strlen(logstring);
	char buffer[strlength];
	sprintf(buffer, logstring);

	/* open log file only if there is not one currently in use */
	if ( ERROR == set_logfile(Name_logfile) ) { 
		return ERROR;
	}

	/* write to log file */
	int val = write(Global_fd, buffer, sizeof(buffer)); 

	if ( strlength != val ) { 
		perror("\n log_event: file-write error! \n"); 
		return ERROR; 
	} 

	/* close log file */
	close_logfile();

	return OK;
}

int set_logfile(const char *logfile_name)
{
	int local_fd;

	/* redefine log file name */

	if ( NULL != logfile_name ) { 
		Name_logfile = logfile_name; 
	} 

	local_fd = open(Name_logfile, O_RDWR | O_CREAT | O_APPEND, 0600);

	if ( 0 > local_fd ) { 
		perror("\n set_logfile: file-open error! \n"); 
		return ERROR; 
	} else { 

		/* close original file descriptor */
		close_logfile();

		/* reassign global file descriptor */
		Global_fd = local_fd;
	}

	return OK;
}

void close_logfile(void)
{
	/* close log file */
	if ( NULLFD != Global_fd ) { 
		if (0 > close(Global_fd) ) { 
			perror("\n close_logfile: error closing file! \n"); 
		} else { 
			Global_fd = NULLFD; 
		} 
	} 

	return;
}
