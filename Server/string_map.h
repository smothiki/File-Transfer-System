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
#ifndef _STRING_MAP_COMPLETE_DEFINED	// To prevent redeclaration
#define _STRING_MAP_COMPLETE_DEFINED

#include "definitions/data_list.h"

#define STRING_MAP_SUCCESSFUL 0
#define STRING_MAP_ELEMENT_NOT_FOUND -1
#define STRING_MAP_INVALID_ARGUMENTS -2

///* This structure represents a single element in 'STRING_MAP' */
struct STRING_MAP_ELEMENT
{
	char indentifier[1024];	// Holds the indentifier by which the element is identified
	char value[1024];		// Holds the value corrosponding to the identifier
};

///* This structure represents a single map */
struct STRING_MAP
{
	int itemCount;		// Holds the number of elements in the map
	struct DATA_LIST lists[50];	// Array of lists to hold the elements
};

int createStringMap(struct STRING_MAP *smap);
int destroyStringMap(struct STRING_MAP *smap);
int addMapElement(const char *iden,const char *val,struct STRING_MAP *smap);
int removeMapElement(const char *iden,struct STRING_MAP *smap);
int getMapElementValue(const char *iden,char *val,struct STRING_MAP *smap);
int setMapElementValue(const char *iden,const char *val,struct STRING_MAP *smap);

#endif
