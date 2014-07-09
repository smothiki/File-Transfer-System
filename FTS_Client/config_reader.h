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
#ifndef _CONFIG_READER_COMPLETE_DEFINED
#define _CONFIG_READER_COMPLETE_DEFINED

#define CONFIG_READER_INVALID_ARGUMENTS -1
#define CONFIG_READER_ENTRY_NOT_FOUND -2
#define CONFIG_READER_CONFIG_FILE_NOT_FOUND -3
#define CONFIG_READER_SUCCESSFUL 0

#include "string_map.h"
//
// The 'CONFIG_READER' reads and gets the configuration information
// in the config file, each configuration item must be on a SEPARATE LINE and
// must be in the format:
//		[Identifier]=[value] ( Note: No spaces on either side of '=' )
// Examples:
//		PATH=/home/wipro/   (Here the 'PATH' is the identifire and '/home/wipro/' is it's value
//		NAME=wipro 123		(Here the value has spaces in it, this is valid)
//		NAME= wipro			(Here there is a space between '=' and the value, THIS IS INVALID)
//		NAME =wipro			(Here there is a space between 'NAME' and =, THIS IS ALSO INVALID)
//
struct CONFIG_READER
{
	char config_file_path[1024];	// Holds the path to the configuration file
	struct STRING_MAP configData;	// Map which holds the configuration data
};

int createConfigReader(struct CONFIG_READER *cfgr,char *path_to_config_file);
int getConfigValue(const char *indentifier,char *value,struct CONFIG_READER *cfgr);
int refeshConfigReader(struct CONFIG_READER *cfgr);
int destroyConfigReader(struct CONFIG_READER *cfgr);
int getConfigCount(struct CONFIG_READER *cfgr);

#endif
