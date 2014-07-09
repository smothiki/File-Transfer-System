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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include "command_interpreter.h"
#include "server_cmd_handlers.h"
#include "cmd_mapper.h"
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
//#include <errno.h>
#define MSGSZ 100
struct msgbuf
{
        long mtype;
        char mtext[MSGSZ];
};
struct CMD_MAP cmd_map[]={
	{"ascii",ChangeMode},
	{"bin",ChangeMode},
	{"rcd",serverChangeDirectory},
	{"rls",serverListDirectory},
	{"rmd",serverMakeDirectory},
	{"rpwd",serverPrintWorkingDirectory},
	{"get",GetFile},
	{"mget",MGetFile},
	{"bye",exitProgram},
	{"put",PutFile},
	{"mput",MPutFile},
	{"",NULL}
};

int writeToLog(char *str)
{
	const int SIZE=100;
	const int KEYGENNUM=9;
	const int PERMS=0600;
	key_t x=ftok("/etc/passwd",KEYGENNUM);
	int mid=msgget(x,IPC_CREAT|PERMS),ret;
	//perror("get");
	struct msgbuf mb;
	int n;
	char buf[SIZE];
	//printf("mid: %d\n",mid);
	mb.mtype=(long)1;
	strncpy(mb.mtext,str,SIZE);
	//printf("before sending\n");
	ret=msgsnd(mid,&mb,sizeof(mb.mtext),0);
	//printf("wtl returning\n");
	return (0);
}

int main(int argc,char *argv[])
{
	const int BUF_SIZE=1024;
	int sockfd;//atoi(argv[1]);
	struct FTS fts;
	char command[BUF_SIZE];
	int n,index;
	struct COMMAND cmd;
	sockfd=atoi(argv[1]);
	fts.sockfd=sockfd;
	fts.mode=FTS_MODE_ASCII;
	//printf("sock %d\n",sockfd);
	//printf("process spawned\n");
	while(1)
	{
		n=read(fts.sockfd,command,BUF_SIZE);
		if(n<=0)
			break;
		command[n]='\0';
		//printf("received command: %s\n",command);
		writeToLog(command);
		//printf("after logging\n");
		fillCommandObject (command,&cmd);
		//printf("after filling\n");
		index=getCommandIndex (&cmd,(struct CMD_MAP *)&cmd_map);
		//printf("index: %d\n");
		cmd_map[index].handler(&cmd,&fts);
		//printf("after handler\n");
		destroyCommandObject (&cmd);
	}
	//printf("process exiting\n");
	return (0);
}

