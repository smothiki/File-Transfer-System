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
#include<stdio.h>
#include<string.h>
#include "authenticator.h"

int getUserAndHomeDir(char *str,char *uname,char *dir);

int authenticateUser(const char *user,char *homeDir)
{
	const int BUF_SIZE=1024;
	const int UNLEN=50;
	char username[UNLEN];
	char dir[BUF_SIZE];
	char buf[BUF_SIZE];
	FILE* f=fopen("/etc/passwd","r");
	if(f==NULL)
	{
		return (AUTHENTICATOR_UNKNOWN_ERROR);
	}
	while(fgets(buf,BUF_SIZE,f))
	{
		getUserAndHomeDir(buf,username,dir);
		if(!strcmp(username,user))
		{
			strncpy(homeDir,dir,BUF_SIZE);
			fclose(f);
			return (AUTHENTICATOR_SUCCESSFUL);
		}
	}
	fclose(f);
	return (AUTHENTICATOR_AUTHENTICATION_FAILED);
}

int getUserAndHomeDir(char *str,char *uname,char *dir)
{
	int i=0,uind=0,dind=0;
	int count=0;
	const int DPOS=5;
	while(str[i]!='\0' && str[i]!='\n')
	{
		if(str[i]==':')
			count++;
		else if(count == 0)
		{
			uname[uind++]=str[i];
			uname[uind]='\0';
		}
		else if(count == DPOS)
		{
			dir[dind++]=str[i];
			dir[dind]='\0';
		}
		i++;
	}
	return (AUTHENTICATOR_SUCCESSFUL);
}
