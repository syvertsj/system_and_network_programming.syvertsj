/**
 * log_mgr.h 
 * 
 * James A. Syvertsen
 */ 

#ifndef LOG_MGR_H
#define LOG_MGR_H

// return macros:
#define OK 0
#define ERROR -1

// default for global file descriptor:
#define NULLFD -99

/* global file descriptor */
extern int Global_fd;

/* default log file declaration (defined in library) */
extern const char *Name_logfile;
 
/* Logging levels */
typedef enum {INFO, WARNING, FATAL} Levels;


/**
 *  Take argument information.
 *  Format it into a time tagged line of text.
 *     <date & time> : <Level> : [PID]<Formatted line of text>
 *  Append it to the current log file (default "logfile" defined in header).
 */
int log_event(Levels l, const char *fmt, ...);  

/**
 * 'creat' or 'open' the log file name passed
 *    if successful, then close the previous log file, and use new file for "log_event()" calls.
 *    -- return OK;
 *    else keep original log file open without losing information in either case.
 *    -- return ERROR;
 */
int set_logfile(const char *logfile_name);

/**
 * close file descriptor associated with an open log file name.
 */
void close_logfile(void);


#endif  /* LOG_MGR_H */
