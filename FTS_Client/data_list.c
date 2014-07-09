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
#include "data_list.h"
#include<string.h>
#include<stdlib.h>

int createDataList(struct DATA_LIST *dl,int dataLen)
{
	if(dl==NULL)
		return (DATA_LIST_INVALID_ARGUMENTS);
	dl->start=dl->end=NULL;
	dl->numNodes=0;
	dl->dataLen=dataLen;
	return (DATA_LIST_SUCCESSFUL);
}

int pushData(void* data,struct DATA_LIST* dl)
{
	if(data == NULL || dl==NULL)
		return (DATA_LIST_INVALID_ARGUMENTS);
	struct DATA_LIST_NODE* dln=(struct DATA_LIST_NODE*)malloc(sizeof(struct DATA_LIST_NODE));
	dln->data=malloc(dl->dataLen);
	if(dln==NULL)
		return (DATA_LIST_INSUFFICIENT_MEMORY);
	dln->next=NULL;
	dln->prev=dl->end;
	memcpy(dln->data,data,dl->dataLen);
	if(dl->end != NULL)
		dl->end->next=dln;
	dl->end=dln;
	if(dl->start==NULL)
		dl->start=dln;
	dl->numNodes++;
	return (DATA_LIST_SUCCESSFUL);
}

int insertDataAt(void* data,struct DATA_LIST_NODE* dln,struct DATA_LIST* dl)
{
	if(data == NULL || dl==NULL || dln==NULL)
		return (DATA_LIST_INVALID_ARGUMENTS);
	struct DATA_LIST_NODE* tdln=(struct DATA_LIST_NODE*)malloc(sizeof(struct DATA_LIST_NODE));
	tdln->data=malloc(dl->dataLen);
	if(tdln==NULL)
		return (DATA_LIST_INSUFFICIENT_MEMORY);
	if(dln->prev != NULL)
	{
		tdln->next=dln->prev->next;
		dln->prev->next->prev=tdln;
		tdln->prev=dln->prev;
		dln->prev->next=tdln;
	}
	else if(dln->next != NULL)
	{
		tdln->next=dln->next->prev;
		tdln->prev=NULL;
		dln->next->prev->prev=tdln;
		dl->start=tdln;
	}
	else if(dl->numNodes == 1)
	{
		tdln->next=dl->start;
		tdln->prev=NULL;
		dl->start->prev=tdln;
		dl->start=tdln;
	}
	else
		return (DATA_LIST_INVALID_ARGUMENTS);
	memcpy(tdln->data,data,dl->dataLen);
	dl->numNodes++;
	return (DATA_LIST_SUCCESSFUL);
}

int removeData(struct DATA_LIST_NODE* dln,struct DATA_LIST* dl)
{
	if(dl==NULL || dln==NULL)
		return (DATA_LIST_INVALID_ARGUMENTS);
	if(dl->numNodes <= 0)
		return (DATA_LIST_INVALID_OPERATION);
	struct DATA_LIST_NODE* tdln;
	if(dln->prev != NULL)
	{
		tdln=dln->prev->next;
		tdln->prev->next=tdln->next;
		if(dln->next == NULL)
		{
			dl->end=tdln->prev;
			dl->end->next=NULL;
		}
		else
		{
			tdln->next->prev=tdln->prev;
		}

	}
	else if(dln->next != NULL)
	{
		tdln=dln->next->prev;
		dl->start=tdln->next;
		tdln->next->prev=NULL;
	}
	else if(dl->numNodes == 1)
	{
		tdln=dl->start;
		dl->start=dl->end=NULL;
	}
	else
		return (DATA_LIST_INVALID_ARGUMENTS);
	free(tdln->data);
	free(tdln);
	dl->numNodes--;
	dln->next=dln->prev=NULL;
	return (DATA_LIST_SUCCESSFUL);
}

int setDataNode(struct DATA_LIST_NODE* newdln,struct DATA_LIST_NODE* dln,struct DATA_LIST* dl)
{
	if(dl==NULL || dln==NULL || newdln==NULL)
		return (DATA_LIST_INVALID_ARGUMENTS);
	if(dl->numNodes <= 0)
		return (DATA_LIST_INVALID_OPERATION);
	struct DATA_LIST_NODE* tdln;
	if(dln->prev != NULL)
	{
		tdln=dln->prev->next;
	}
	else if(dln->next != NULL)
	{
		tdln=dln->next->prev;
	}
	else if(dl->numNodes == 1)
	{
		tdln=dl->start;
	}
	else
		return (DATA_LIST_INVALID_ARGUMENTS);
	memcpy(tdln->data,newdln->data,dl->dataLen);
	return (DATA_LIST_SUCCESSFUL);
}

int getDataNode(int index,struct DATA_LIST_NODE* dln,struct DATA_LIST* dl)
{
	int i;
	if(dl==NULL || dln==NULL)
		return (DATA_LIST_INVALID_ARGUMENTS);
	if(dl->numNodes <= index)
		return (DATA_LIST_INDEX_OUT_OF_BOUNDS);
	struct DATA_LIST_NODE* tdln=dl->start;
	for(i=0;i<index;i++)
		tdln=tdln->next;
	dln->next=tdln->next;
	dln->prev=tdln->prev;
	memcpy(dln->data,tdln->data,dl->dataLen);
	return (DATA_LIST_SUCCESSFUL);
}

int getNextDataNode(struct DATA_LIST_NODE* dln,struct DATA_LIST* dl)
{
	struct DATA_LIST_NODE newdln;
	if(dln==NULL || dl==NULL)
		return (DATA_LIST_INVALID_ARGUMENTS);
	if(dln->next == NULL)
		return (DATA_LIST_END_REACHED);
	newdln=*(dln->next);
	dln->next=newdln.next;
	dln->prev=newdln.prev;
	memcpy(dln->data,newdln.data,dl->dataLen);
	return (DATA_LIST_SUCCESSFUL);
}

int getPreviousDataNode(struct DATA_LIST_NODE* dln,struct DATA_LIST* dl)
{
	struct DATA_LIST_NODE newdln;
	if(dln==NULL || dl==NULL)
		return (DATA_LIST_INVALID_ARGUMENTS);
	if(dln->prev == NULL)
		return (DATA_LIST_START_REACHED);
	newdln=*(dln->prev);
	dln->next=newdln.next;
	dln->prev=newdln.prev;
	memcpy(dln->data,newdln.data,dl->dataLen);
	return (DATA_LIST_SUCCESSFUL);
}

int destroyDataList(struct DATA_LIST* dl)
{
	if(dl == NULL)
		return (DATA_LIST_INVALID_ARGUMENTS);
	struct DATA_LIST_NODE tdln;
	while(dl->numNodes != 0)
	{
		tdln=*(dl->start);
		removeData(&tdln,dl);
	}
	return (DATA_LIST_SUCCESSFUL);
}

int createDataListNode(struct DATA_LIST_NODE* dln,struct DATA_LIST* dl)
{
	dln->next=dln->prev=NULL;
	dln->data=malloc(dl->dataLen);
	return (DATA_LIST_SUCCESSFUL);
}
int destroyDataListNode(struct DATA_LIST_NODE* dln)
{
	free(dln->data);
	dln->next=dln->prev=NULL;
	return (DATA_LIST_SUCCESSFUL);
}
