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
#ifndef _DATA_LIST_DEFINED	// To prevent redeclaration of 'DATA_LIST' structure
#define _DATA_LIST_DEFINED

///* This structure represents a list */
struct DATA_LIST
{
	int numNodes;					// Number of nodes in the list
	int dataLen;					// Holds the size of the data
	struct DATA_LIST_NODE* start;	// Points to the start of the list
	struct DATA_LIST_NODE* end;		// Points to the end of the list
};

#endif
