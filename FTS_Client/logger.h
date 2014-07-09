//
// Filename
//
// Description
//
//
// Copyright (c) <date> <company>
// All Rights Reserved
//
// Author:
//    Authors
//
// General Comments
//
//
// $Header: $
//
//
// $Log: $
//
#ifndef _LOGGER_COMPLETE_DEFINED	// To prevent redeclaration
#define _LOGGER_COMPLETE_DEFINED

#define LOGGER_DIR_NOT_FOUND -1
#define LOGGER_FILE_CREATION_FAILED -2
#define LOGGER_FILE_OPENING_FAILED -3
#define LOGGER_WRITING_FAILED -4
#define LOGGER_INVALID_ARGUMENTS -5
#define LOGGER_SUCCESSFUL 0

typedef int bool;
#define TRUE 1
#define FALSE 0
///* This structure represents a single logger */
struct LOGGER
{
	char directory_path[1024];		// Contains the path to the directory where logfiles are created
	char logFileName[100];			// Holds the name of the log file. a number is appended to the name when creating files. ex: lf1,lf2,...,lf10
	int maxLogFileCount;			// Holds the LogFile count limit.
	unsigned int maxLogFileSize;	// Holds the LogFile size limit
	int currentLog;					// Holds the current logfile number
	int logFileDescriptor;			// Holds the current logfile file descriptor
	int currentFileSize;			// Holds the size of the current log file
	bool writeable;					// Is true if logger is ready to log data. If not it is false.
};

int createLogger(struct LOGGER *lg,const char *log_file_name,const char *directory_path,const int maxLogFileCount,const unsigned int maxLogFileSize);
int destroyLogger(struct LOGGER *lg);
int writeToLogFile(const char *str,struct LOGGER *lg);

#endif
