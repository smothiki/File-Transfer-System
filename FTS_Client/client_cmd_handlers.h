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
#include "definitions/fts.h"
#define CHANDLER_INCORRECT_NO_OF_ARGUMENTS -1
#define CHANDLER_UNEXPECTED_ERROR -2
#define CHANDLER_OPERATION_FAILED -3
#define CHANDLER_SUCCESSFUL 0

int clientPrintHelp(struct COMMAND *cmd,struct FTS *fts);
int ChangeMode(struct COMMAND *cmd,struct FTS *fts);
int exitProgram(struct COMMAND *cmd,struct FTS *fts);
int clientChangeDirectory(struct COMMAND *cmd,struct FTS *fts);
int serverChangeDirectory(struct COMMAND *cmd,struct FTS *fts);
int clientListDirectory(struct COMMAND *cmd,struct FTS *fts);
int serverListDirectory(struct COMMAND *cmd,struct FTS *fts);
int clientMakeDirectory(struct COMMAND *cmd,struct FTS *fts);
int serverMakeDirectory(struct COMMAND *cmd,struct FTS *fts);
int clientPrintWorkingDirectory(struct COMMAND *cmd,struct FTS *fts);
int serverPrintWorkingDirectory(struct COMMAND *cmd,struct FTS *fts);
int GetFile(struct COMMAND *cmd,struct FTS *fts);
int MGetFile(struct COMMAND *cmd,struct FTS *fts);
int PutFile(struct COMMAND *cmd,struct FTS *fts);
int MPutFile(struct COMMAND *cmd,struct FTS *fts);
