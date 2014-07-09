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
#include "command_interpreter.h"
#include<string.h>
#include<stdlib.h>

int getTokenCount(const char *str);

int fillCommandObject(const char *str,struct COMMAND *cmd)
{
	const int BUF_SIZE=1024;
	int const CMD=50;
	int tokCount=getTokenCount(str);
	const char* delim=" ";
	char* result;
	int count=0;
	if(tokCount == 0)
	{
		cmd->cmd[0]='\0';
		cmd->numArgs=0;
		cmd->args=NULL;
		return (CI_INVALID_INPUT_STRING);
	}
	cmd->numArgs=tokCount-1;
	cmd->args=(char **)malloc(sizeof(char *)*tokCount);
	cmd->args[tokCount-1]=NULL;
	result=strtok(str,delim);
	while(result != NULL)
	{
		if(count == 0)
		{
			strncpy(cmd->cmd,result,CMD);
		}
		else
		{
			cmd->args[count-1]=(char *) malloc(strlen(result)+3);
			strncpy(cmd->args[count-1],result,strlen(result)+3);
		}
		count++;
		result=strtok(NULL,delim);
	}
	return (CI_SUCCESSFUL);
}

int destroyCommandObject(struct COMMAND *cmd)
{
	int i;
	for(i=0;i<cmd->numArgs;i++)
		free(cmd->args[i]);
	free(cmd->args);
	cmd->numArgs=0;
	return (CI_SUCCESSFUL);
}

int getTokenCount(const char *str)
{
	int i=0;
	int count=0;
	while(str[i]!='\0')
	{
		if(count == 0)
		{
			if(str[i]!=' ')
				count++;
		}
		else
		{
			if(str[i]==' ')
			{
				if(str[i+1]!=' ' && str[i+1]!='\0')
				count++;
			}
		}
		i++;
	}
	return (count);
}
