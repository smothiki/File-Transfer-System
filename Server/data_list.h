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
#ifndef _DATA_LIST_COMPLETE_DEFINED
#define _DATA_LIST_COMPLETE_DEFINED

#define DATA_LIST_INVALID_ARGUMENTS -1
#define DATA_LIST_INSUFFICIENT_MEMORY -2
#define DATA_LIST_INVALID_OPERATION -3
#define DATA_LIST_END_REACHED -4
#define DATA_LIST_START_REACHED -5
#define DATA_LIST_INDEX_OUT_OF_BOUNDS -6
#define DATA_LIST_SUCCESSFUL 0

#include "definitions/data_list.h"

///* This structure represents a node in 'DATA_LIST' list */
struct DATA_LIST_NODE
{
	void* data;						// Holds the address to Data
	struct DATA_LIST_NODE* next;	// Points to the next node in the list
	struct DATA_LIST_NODE* prev;	// Points to the previous node in the list
};



int createDataList(struct DATA_LIST *dl,int dataLen);
int pushData(void *data,struct DATA_LIST *dl);
int insertDataAt(void *data,struct DATA_LIST_NODE *dln,struct DATA_LIST *dl);
int removeData(struct DATA_LIST_NODE *dln,struct DATA_LIST *dl);
int getDataNode(int index,struct DATA_LIST_NODE *dln,struct DATA_LIST *dl);
int getNextDataNode(struct DATA_LIST_NODE *dln,struct DATA_LIST *dl);
int getPreviousDataNode(struct DATA_LIST_NODE *dln,struct DATA_LIST *dl);
int destroyDataList(struct DATA_LIST *dl);
int createDataListNode(struct DATA_LIST_NODE *dln,struct DATA_LIST *dl);
int destroyDataListNode(struct DATA_LIST_NODE *dln);
int setDataNode(struct DATA_LIST_NODE *newdln,struct DATA_LIST_NODE *dln,struct DATA_LIST *dl);


#endif
