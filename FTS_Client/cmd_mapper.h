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
#include "definitions/command_struct.h"
#include "definitions/fts.h"

#define CMD_MAP_COMMAND_NOT_FOUND -1

///* This structure maps a single command to a single argument */
struct CMD_MAP
{
	char cmd[100];
	int (*handler)(struct COMMAND *cmd,struct FTS *fts);
};

int getCommandIndex(struct COMMAND *cmd,struct CMD_MAP *map);
