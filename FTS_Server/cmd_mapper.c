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
#include "cmd_mapper.h"
#include<string.h>
//
//	This function takes a COMMAND variable and a pointer to and array of CMD_MAP's.
//	The last element in the array must have it's handler address as NULL.
//	The function returns the index of the command in the array if exists, else
// it returns an error code.
//
int getCommandIndex(struct COMMAND *cmd,struct CMD_MAP *map)
{
	int i=0;
	while(map[i].handler != NULL)
	{
		if(!strcmp(map[i].cmd,cmd->cmd))
		{
			return (i);
		}
		i++;
	}
	return (CMD_MAP_COMMAND_NOT_FOUND);
}
