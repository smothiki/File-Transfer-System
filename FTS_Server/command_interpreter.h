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

#define CI_INVALID_INPUT_STRING -1
#define CI_SUCCESSFUL 0

int fillCommandObject(const char *str,struct COMMAND *cmd);
int destroyCommandObject(struct COMMAND *cmd);
