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
#include "logger.h"
#include<stdio.h>
#include<fcntl.h>
#include<string.h>

int createLogger(struct LOGGER *lg,const char *log_file_name,const char *directory_path,const int maxLogFileCount,const unsigned int maxLogFileSize)
{
	const int BUF_SIZE=1024;
	int fd,i;
	const int PERMS=0644;
	if(lg==NULL || strlen(log_file_name)<=0 || strlen(directory_path)<=0 || maxLogFileCount <= 0 || maxLogFileSize == 0)
		return (LOGGER_INVALID_ARGUMENTS);
	lg->currentLog=1;
	strncpy(lg->logFileName,log_file_name,BUF_SIZE);
	lg->maxLogFileCount=maxLogFileCount;
	lg->maxLogFileSize=maxLogFileSize;
	lg->writeable=FALSE;
	char tmp[BUF_SIZE],tmp2[BUF_SIZE];
	getcwd(tmp2,BUF_SIZE);					//get current working dirc
	if(chdir(directory_path) < 0)
		return (LOGGER_DIR_NOT_FOUND);
	getcwd(tmp,BUF_SIZE);
	strncpy(lg->directory_path,tmp,BUF_SIZE);
	chdir(tmp2);
	for(i=1;i<=maxLogFileCount;i++)
	{
		snprintf(tmp2,BUF_SIZE,"%s/%s%d",tmp,log_file_name,i);
		fd=open(tmp2,O_CREAT | O_WRONLY |O_TRUNC,PERMS);
		if(fd < 0)
		{
			if(lg->writeable)
			{
				close(lg->logFileDescriptor);
				lg->currentLog=0;
				lg->writeable=FALSE;
			}
			return (LOGGER_FILE_CREATION_FAILED);
		}
		if(i==1)
		{
			lg->logFileDescriptor=fd;
			lg->currentFileSize=0;
			lg->currentLog=1;
			lg->writeable=TRUE;
		}
		else
			close(fd);
	}
	return (LOGGER_SUCCESSFUL);
}

int destroyLogger(struct LOGGER *lg)
{
	if(lg==NULL)
		return (LOGGER_INVALID_ARGUMENTS);
	if(lg->writeable)
	{
		close(lg->logFileDescriptor);
		lg->writeable=FALSE;
	}
	return (LOGGER_SUCCESSFUL);
}

int writeToLogFile(const char *str,struct LOGGER *lg)
{
	const int BUF_SIZE=1024;
	if(!lg->writeable)
		return (LOGGER_WRITING_FAILED);
	int len=strlen(str)+1;
	if(len == 1)
		return (LOGGER_INVALID_ARGUMENTS);
	if(lg->currentFileSize+len > lg->maxLogFileSize)
	{
		close(lg->logFileDescriptor);
		if(lg->currentLog == lg->maxLogFileCount)
			lg->currentLog=1;
		else
			lg->currentLog++;
		char buf[BUF_SIZE];
		snprintf(buf,BUF_SIZE,"%s/%s%d",lg->directory_path,lg->logFileName,lg->currentLog);
		lg->logFileDescriptor=open(buf,O_WRONLY | O_TRUNC);
		if(lg->logFileDescriptor < 0)
		{
			lg->writeable=FALSE;
			return (LOGGER_FILE_OPENING_FAILED);
		}
		lg->currentFileSize=0;
	}
	char buf[BUF_SIZE];
	lg->currentFileSize+=snprintf(buf,BUF_SIZE,"%s\n",str);
	if(write(lg->logFileDescriptor,buf,len) < 0)
		return (LOGGER_WRITING_FAILED);
	return (LOGGER_SUCCESSFUL);
}
