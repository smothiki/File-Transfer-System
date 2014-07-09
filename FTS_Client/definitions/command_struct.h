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

#ifndef __DEFINED_COMMAND_	// Prevents redeclaration of 'struct COMMAND'

#define __DEFINED_COMMAND_
///* This structure represents a single command */
struct COMMAND
{
	char cmd[100];	// Holds the command
	char** args;	// Holds the arguments
	int numArgs;	// Holds the number of arguments
};

#endif
