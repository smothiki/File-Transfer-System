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
#include "string_map.h"
#include "data_list.h"
#include<string.h>
#include<stdlib.h>

int getHashValue(const char *str)
{
	const int MAX_ARR_SIZE=50;
	unsigned int x=1;
	int i;
	int len=strlen(str);
	for(i=0;i<len;i++)
		x*=(unsigned int)str[i]+(unsigned int)i;
	return ((int)(x%MAX_ARR_SIZE));
}

int createStringMap(struct STRING_MAP *smap)
{
	int i;
	const int MAX_ARR_SIZE=50;
	if(smap == NULL)
		return (STRING_MAP_INVALID_ARGUMENTS);
	for(i=0;i<MAX_ARR_SIZE;i++)
	{
		createDataList(&smap->lists[i],sizeof(struct STRING_MAP_ELEMENT));
	}
	smap->itemCount=0;
	return (STRING_MAP_SUCCESSFUL);
}

int destroyStringMap(struct STRING_MAP *smap)
{
	int i;
	const int MAX_ARR_SIZE=50;
	if(smap==NULL)
		return (STRING_MAP_INVALID_ARGUMENTS);
	smap->itemCount=0;
	for(i=0;i<MAX_ARR_SIZE;i++)
	{
		destroyDataList(&smap->lists[i]);
	}
	return (STRING_MAP_SUCCESSFUL);
}

int addMapElement(const char *iden,const char *val,struct STRING_MAP *smap)
{
	const int BUF_SIZE=1024;
	int hash;
	struct DATA_LIST_NODE node;
	struct STRING_MAP_ELEMENT* ele;
	if(iden==NULL || val==NULL || smap==NULL)
		return (STRING_MAP_INVALID_ARGUMENTS);
	hash=getHashValue(iden);
	createDataListNode(&node,&smap->lists[hash]);
	getDataNode(0,&node,&smap->lists[hash]);
	ele=((struct STRING_MAP_ELEMENT *) node.data);
	if(!strcmp(iden,ele->indentifier))
	{
		strncpy(ele->value,val,BUF_SIZE);
		setDataNode(&node,&node,&smap->lists[hash]);
		destroyDataListNode(&node);
		return (STRING_MAP_SUCCESSFUL);
	}
	while(getNextDataNode(&node,&smap->lists[hash])!=DATA_LIST_END_REACHED)
	{
		ele=((struct STRING_MAP_ELEMENT *) node.data);
		if(!strcmp(iden,ele->indentifier))
		{
			strncpy(ele->value,val,BUF_SIZE);
			setDataNode(&node,&node,&smap->lists[hash]);
			destroyDataListNode(&node);
			return (STRING_MAP_SUCCESSFUL);
		}
	}
	ele=(struct STRING_MAP_ELEMENT *)malloc(sizeof(struct STRING_MAP_ELEMENT));
	strncpy(ele->indentifier,iden,BUF_SIZE);
	strncpy(ele->value,val,BUF_SIZE);
	pushData(ele,&smap->lists[hash]);
	smap->itemCount++;
	destroyDataListNode(&node);
	return (STRING_MAP_SUCCESSFUL);
}

int removeMapElement(const char *iden,struct STRING_MAP *smap)
{
	int hash;
	struct STRING_MAP_ELEMENT* ele;
	struct DATA_LIST_NODE node;
	if(iden==NULL || smap==NULL)
		return (STRING_MAP_INVALID_ARGUMENTS);
	hash=getHashValue(iden);
	createDataListNode(&node,&smap->lists[hash]);
	getDataNode(0,&node,&smap->lists[hash]);
	ele=((struct STRING_MAP_ELEMENT *) node.data);
	if(!strcmp(iden,ele->indentifier))
	{
		removeData(&node,&smap->lists[hash]);
		smap->itemCount--;
		destroyDataListNode(&node);
		return (STRING_MAP_SUCCESSFUL);
	}
	while(getNextDataNode(&node,&smap->lists[hash])!=DATA_LIST_END_REACHED)
	{
		ele=((struct STRING_MAP_ELEMENT *) node.data);
		if(!strcmp(iden,ele->indentifier))
		{
			removeData(&node,&smap->lists[hash]);
			smap->itemCount--;
			destroyDataListNode(&node);
			return (STRING_MAP_SUCCESSFUL);
		}
	}
	destroyDataListNode(&node);
	return (STRING_MAP_ELEMENT_NOT_FOUND);
}

int getMapElementValue(const char *iden,char *val,struct STRING_MAP *smap)
{
	const int BUF_SIZE=1024;
	int hash;
	struct DATA_LIST_NODE node;
	struct STRING_MAP_ELEMENT* ele;
	if(iden==NULL || val==NULL || smap==NULL)
		return (STRING_MAP_INVALID_ARGUMENTS);
	hash=getHashValue(iden);
	createDataListNode(&node,&smap->lists[hash]);
	getDataNode(0,&node,&smap->lists[hash]);
	ele=((struct STRING_MAP_ELEMENT *) node.data);
	if(!strcmp(iden,ele->indentifier))
	{
		strncpy(val,ele->value,BUF_SIZE);
		destroyDataListNode(&node);
		return (STRING_MAP_SUCCESSFUL);
	}
	while(getNextDataNode(&node,&smap->lists[hash])!=DATA_LIST_END_REACHED)
	{
		ele=((struct STRING_MAP_ELEMENT *) node.data);
		if(!strcmp(iden,ele->indentifier))
		{
			strncpy(val,ele->value,BUF_SIZE);
			destroyDataListNode(&node);
			return (STRING_MAP_SUCCESSFUL);
		}
	}
	destroyDataListNode(&node);
	return (STRING_MAP_ELEMENT_NOT_FOUND);
}
int setMapElementValue(const char *iden,const char *val,struct STRING_MAP *smap)
{
	const int BUF_SIZE=1024;
	int hash;
	struct DATA_LIST_NODE node;
	struct STRING_MAP_ELEMENT* ele;
	if(iden==NULL || val==NULL || smap==NULL)
		return (STRING_MAP_INVALID_ARGUMENTS);
	hash=getHashValue(iden);
	createDataListNode(&node,&smap->lists[hash]);
	getDataNode(0,&node,&smap->lists[hash]);
	ele=((struct STRING_MAP_ELEMENT *) node.data);
	if(!strcmp(iden,ele->indentifier))
	{
		strncpy(ele->value,val,BUF_SIZE);
		setDataNode(&node,&node,&smap->lists[hash]);
		destroyDataListNode(&node);
		return (STRING_MAP_SUCCESSFUL);
	}
	while(getNextDataNode(&node,&smap->lists[hash])!=DATA_LIST_END_REACHED)
	{
		ele=((struct STRING_MAP_ELEMENT *) node.data);
		if(!strcmp(iden,ele->indentifier))
		{
			strncpy(ele->value,val,BUF_SIZE);
			setDataNode(&node,&node,&smap->lists[hash]);
			destroyDataListNode(&node);
			return (STRING_MAP_SUCCESSFUL);
		}
	}
	destroyDataListNode(&node);
	return (STRING_MAP_ELEMENT_NOT_FOUND);
}
