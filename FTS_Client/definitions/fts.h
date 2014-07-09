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
#ifndef __DEFINED_FTS_	// Prevents redeclaration of 'struct FTS'

#define __DEFINED_FTS_
#define FTS_MODE_BINARY 1
#define FTS_MODE_ASCII 2

struct FTS
{
	int mode;
	int sockfd;
};
#endif
