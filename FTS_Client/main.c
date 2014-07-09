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
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "cmd_mapper.h"
#include "client_cmd_handlers.h"
#include "command_interpreter.h"
#include "logger.h"
#include <signal.h>
#include "config_reader.h"

struct CMD_MAP cmd_map[]={
	{"ascii",ChangeMode},
	{"bin",ChangeMode},
	{"lcd",clientChangeDirectory},
	{"lls",clientListDirectory},
	{"lmd",clientMakeDirectory},
	{"lpwd",clientPrintWorkingDirectory},
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

int connectToServer(char *ip)
{
	struct sockaddr_in serv;
	int sockfd;
	struct in_addr ad;
	const int PORT=2050;
	socklen_t len;
	inet_pton(AF_INET,ip,&ad);
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_addr.s_addr=ad.s_addr;
	serv.sin_port=htons(PORT);
	serv.sin_family=AF_INET;
	len=sizeof(struct sockaddr_in);
	if(connect(sockfd,(struct sockaddr *)&serv,len) < 0)
	{
		puts("Host not active");
		exit(0);
	}
	return (sockfd);
}

struct LOGGER *llg;
void handle(int x)
{
	x=x;
	destroyLogger(llg);
	exit(0);
}

int main(int argc,char *argv[])
{
	const int BUF_SIZE=1024;
	const int CMDLEN=50;
	int sockfd;//atoi(argv[1]);
	struct FTS fts;
	char command[BUF_SIZE];
	int index;
	struct COMMAND cmd;
	char user[CMDLEN];
	char arg[BUF_SIZE];
	const char* prompt="FTS> ";
	int numlogs,logfilesize;
	struct CONFIG_READER cfg;
	struct LOGGER lg;
	char ip[CMDLEN];
	llg=&lg;
	signal(SIGKILL,handle);
	createConfigReader(&cfg,"config.cfg");
	//printf("after config reader\n");
	getConfigValue("NLOGFILES",arg,&cfg);
	numlogs=atoi(arg);
	getConfigValue("LOGFILESIZE",arg,&cfg);
	logfilesize=atoi(arg);
	destroyConfigReader(&cfg);
	//printf("after destroy config reader\n");
	createLogger(&lg,"client_log","./client_logs",numlogs,logfilesize);
	//printf("after create logger\n");
	strncpy(arg,argv[1],BUF_SIZE);
	//printf("arg: %s\n",arg);
	strncpy(user,strtok(arg,"@"),BUF_SIZE);
	//printf("user: %s\n",user);
	strncpy(ip,strtok(NULL,"@"),BUF_SIZE);
	//printf("before connections\n");
	fts.sockfd=sockfd=connectToServer(ip);
	fts.mode=FTS_MODE_ASCII;
	//printf("sock: %d\n",sockfd);
	//printf("user: %s\n",user);
	write(sockfd,user,strlen(user));
	read(sockfd,&index,sizeof(index));
	//printf("%d\n",index);
	if(index==-1)
	{
		puts("user not found");
		exit(0);
	}
	puts("connected successfully");
	while(1)
	{
		printf("%s",prompt);
		fflush(stdout);
		fgets(command,BUF_SIZE,stdin);
		while(command[0]=='\0' || command[0]==' ' || command[0]=='\n')
			fgets(command,BUF_SIZE,stdin);
		if(command[strlen(command)-1]=='\n')
			command[strlen(command)-1]='\0';
		//printf("cmd: %s\n",command);
		writeToLogFile(command,&lg);
		fillCommandObject(command,&cmd);
		index=getCommandIndex(&cmd,(struct CMD_MAP *)&cmd_map);
		if(index < 0)
		{
			puts("Command not found");
			continue;
		}
		cmd_map[index].handler(&cmd,&fts);
		destroyCommandObject (&cmd);
	}
	return (0);
}

