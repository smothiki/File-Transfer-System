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
#include "config_reader.h"
#include<stdio.h>
#include<string.h>
int splitConfigEntry(char *iden,char *val,char *string)
{
	int id=0,v=0;
	int len=strlen(string);
	int i=0;
	for(i=0;i<len;i++)
	{
		if(string[i]!= '=')
		{
			iden[id++]=string[i];
		}
		else
		{
			i++;
			break;
		}
	}
	iden[id]='\0';
	if(i==len)
		return (CONFIG_READER_INVALID_ARGUMENTS);
	for(;i<len;i++)
	{
		if(string[i]!= '\0' && string[i]!= '\n')
		{
			val[v++]=string[i];
		}
		else
		{
			break;
		}
	}
	val[v]='\0';
	return (CONFIG_READER_SUCCESSFUL);
}

int createConfigReader(struct CONFIG_READER *cfgr,char *path_to_config_file)
{
	const int BUFLEN=1024;
	if(cfgr==NULL || path_to_config_file==NULL)
		return (CONFIG_READER_INVALID_ARGUMENTS);
	strncpy(cfgr->config_file_path,path_to_config_file,BUFLEN);
	createStringMap(&cfgr->configData);
	FILE* f=fopen(path_to_config_file,"r");
	if(f==NULL)
		return (CONFIG_READER_CONFIG_FILE_NOT_FOUND);
	char buf[BUFLEN];
	char iden[BUFLEN],val[BUFLEN];
	while(fgets(buf,BUFLEN,f))
	{
		if(splitConfigEntry(iden,val,buf)==0)
		{
			addMapElement(iden,val,&cfgr->configData);
		}
	}
	fclose(f);
	return (CONFIG_READER_SUCCESSFUL);
}
int getConfigValue(const char *indentifier,char *value,struct CONFIG_READER *cfgr)
{
	if(getMapElementValue(indentifier,value,&cfgr->configData) < 0)
		return (CONFIG_READER_ENTRY_NOT_FOUND);
	return (CONFIG_READER_SUCCESSFUL);
}
int refeshConfigReader(struct CONFIG_READER *cfgr)
{
	const int BUF_SIZE=1024;
	destroyStringMap(&cfgr->configData);
	createStringMap(&cfgr->configData);
	FILE* f=fopen(cfgr->config_file_path,"r");
	if(f==NULL)
		return (CONFIG_READER_CONFIG_FILE_NOT_FOUND);
	char buf[BUF_SIZE];
	char iden[BUF_SIZE],val[BUF_SIZE];
	while(fgets(buf,BUF_SIZE,f))
	{
		if(splitConfigEntry(iden,val,buf)==0)
		{
			addMapElement(iden,val,&cfgr->configData);
		}
	}
	fclose(f);
	return (CONFIG_READER_SUCCESSFUL);
}
int destroyConfigReader(struct CONFIG_READER *cfgr)
{
	destroyStringMap(&cfgr->configData);
	return (CONFIG_READER_SUCCESSFUL);
}

int getConfigCount(struct CONFIG_READER *cfgr)
{
	return (cfgr->configData.itemCount);
}
