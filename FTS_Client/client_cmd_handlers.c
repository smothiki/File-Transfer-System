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
#include "client_cmd_handlers.h"
#include "command_interpreter.h"
#include <stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<dirent.h>
#include<signal.h>

int sendCommand(struct COMMAND* cmd,int sockfd)
{
	const int BUF_SIZE=1024;
	char buf[BUF_SIZE];
	int i=0;
	strncpy(buf,cmd->cmd,BUF_SIZE);
	for(i=0;i<cmd->numArgs;i++)
	{
		strncat(buf," ",BUF_SIZE);
		strncat(buf,cmd->args[i],BUF_SIZE);
	}
	//printf("%s\n",buf);
	return ((int)write(sockfd,buf,strlen(buf)));
}

int receiveCommand(struct COMMAND* cmd,struct FTS* fts)
{
	const int BUF_SIZE=1024;
	char buf[BUF_SIZE];
	int n;
	n=read(fts->sockfd,buf,BUF_SIZE);
	buf[n]='\0';
	fillCommandObject(buf,cmd);
	return (CHANDLER_SUCCESSFUL);
}

int receiveResponseCode(int sockfd)
{
	int x;
	read(sockfd,&x,sizeof(x));
	return (x);
}

int printResponseText(int sockfd)
{
	char buf[2];
	int n;
	while((n=read(sockfd,buf,1)) > 0)
	{
		if(buf[0]=='\0')
			break;
		else
			putchar(buf[0]);
	}
	return (CHANDLER_SUCCESSFUL);
}

int writeEOFToSocket(int sockfd)
{
	char c='\0';
	write(sockfd,&c,1);
	return(CHANDLER_SUCCESSFUL);
}

int expandCommand(struct COMMAND* cmd)
{
	const int BUF_SIZE=1024;
	int i;
	char buf[BUF_SIZE];
	DIR* d=NULL;
	int count=0;
	int flag=0;
	char** arr;
	int newcount;
	struct stat filestat;
	struct dirent* ent=NULL;
	//printf("addr: %x\n",cmd->args);
	for(i=0;i<cmd->numArgs;i++)
	{
		if(!strcmp(cmd->args[i],"*"))
		{
			flag=1;
			break;
		}
	}
	if(!flag)
		return (0);
	getcwd(buf,BUF_SIZE);
	d=opendir(buf);
	while((ent=readdir(d))!=NULL)
	{
		stat(ent->d_name,&filestat);
		if(S_ISREG(filestat.st_mode))
			count++;
	}
	closedir(d);
	//printf("count: %d\n",count);
	newcount=count + cmd->numArgs;
	arr=(char**)malloc(sizeof(char *) * newcount);
	{
		int j;
		int k,l;
		for(j=0;j<i;j++)
		{
			arr[j]=cmd->args[j];
		}
		k=j+1;
		d=opendir(buf);
		while((ent=readdir(d))!=NULL)
		{
			stat(ent->d_name,&filestat);
			if(S_ISREG(filestat.st_mode))
			{
				arr[j]=(char *)malloc(strlen(ent->d_name)+5);
				strncpy(arr[j],ent->d_name,strlen(ent->d_name)+5);
				j++;
			}
		}
		for(l=k;l<cmd->numArgs;l++)
		{
			arr[j++]=cmd->args[k];
		}
		arr[newcount-1]=NULL;
		closedir(d);
	}
	cmd->numArgs=newcount - 1;
	//printf("\n\naddr: %x\n",cmd->args);
	free(cmd->args);
	cmd->args=arr;
	return(CHANDLER_SUCCESSFUL);
}

int sendFileInBinaryMode(int sockfd,FILE* f)
{
	const int BUF_LEN=1024;
	unsigned long len;
	const double CENT=100.0;
	char buf[BUF_LEN];
	int i,n;
	fseek(f,0,SEEK_END);
	len=ftell(f);
	//printf("length: %lu\n",len);
	write(sockfd,&len,sizeof(len));
	fseek(f,0,SEEK_SET);
	for(i=0;i<len;)
	{
		n=fread(buf,BUF_LEN,1,f);
		write(sockfd,buf,ftell(f)-i);
		i=ftell(f);
		displayProgressBar((int)((i * CENT) / len));

	}
	puts("");
	return (CHANDLER_SUCCESSFUL);
}

int sendFileInAsciiMode(int sockfd,FILE* f)
{
	const double CENT=100.0;
	int buf;
	char c;
	int len;
	int count=0;
	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);
	write(sockfd,&len,sizeof(len));
	while((buf=fgetc(f))!= EOF)
	{
		c=(char)buf;
		if(c=='\0')
			break;
		else
		{
			write(sockfd,&c,1);
			count++;
			displayProgressBar((int)((count * CENT) / len));
		}
	}
	writeEOFToSocket(sockfd);
	puts("");
	return (CHANDLER_SUCCESSFUL);
}

int writeAsciiResponseToFile(int sockfd,FILE* f)
{
	char buf[2];
	const double CENT=100.0;
	int n;
	int len;
	int count=0;
	read(sockfd,&len,sizeof(len));
	while((n=read(sockfd,buf,1)) > 0)
	{
		if(buf[0]=='\0')
			break;
		else
		{
			fputc(buf[0],f);
			count++;
			displayProgressBar((int)((count * CENT) / len));
		}
	}
	puts("");
	return (CHANDLER_SUCCESSFUL);
}

int writeBinaryResponseToFile(int sockfd,FILE* f)
{
	unsigned long len;
	const int BUF_SIZE=1024;
	const double CENT=100.0;
	char buf[BUF_SIZE];
	int n,i;
	read(sockfd,&len,sizeof(len));
	//printf("length: %lu\n",len);
	for(i=0;i<len;)
	{
		n=read(sockfd,buf,BUF_SIZE);
		fwrite(buf,n,1,f);
		i+=n;
		displayProgressBar((int)((i * CENT) / len));
	}
	puts("");
	return (CHANDLER_SUCCESSFUL);
}

int sendResponseCode(int sockfd,int code)
{
	write(sockfd,&code,sizeof(code));
	return (CHANDLER_SUCCESSFUL);
}

int ChangeMode(struct COMMAND* cmd,struct FTS* fts)
{
	int ret;
	if(!strcmp(cmd->cmd,"ascii"))
	{
		sendCommand(cmd,fts->sockfd);
		ret=receiveResponseCode(fts->sockfd);
		if(ret != CHANDLER_SUCCESSFUL)
			return (ret);
		fts->mode=FTS_MODE_ASCII;
		return (CHANDLER_SUCCESSFUL);
	}
	else if(!strcmp(cmd->cmd,"bin"))
	{
		sendCommand(cmd,fts->sockfd);
		ret=receiveResponseCode(fts->sockfd);
		if(ret != CHANDLER_SUCCESSFUL)
			return (ret);
		fts->mode=FTS_MODE_BINARY;
		return (CHANDLER_SUCCESSFUL);
	}
	else
		return (CHANDLER_UNEXPECTED_ERROR);
}

int exitProgram(struct COMMAND* cmd,struct FTS* fts)
{
	sendCommand (cmd,fts->sockfd);
	kill(getpid(),SIGKILL);
}

int clientChangeDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	fts->mode=fts->mode;
	if(cmd->numArgs != 1)
	{
		puts("Incorrect number of arguments. Type 'help' for more details");
		return (CHANDLER_INCORRECT_NO_OF_ARGUMENTS);
	}
	if(chdir(cmd->args[0]) < 0)
	{
		printf("%s : No such directory found\n",cmd->args[0]);
		return (CHANDLER_OPERATION_FAILED);
	}
	else
		return (CHANDLER_SUCCESSFUL);
}

int serverChangeDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	int ret;
	if(cmd->numArgs != 1)
	{
		puts("Incorrect number of arguments. Type 'help' for more details");
		return (CHANDLER_INCORRECT_NO_OF_ARGUMENTS);
	}
	sendCommand(cmd,fts->sockfd);
	ret=receiveResponseCode(fts->sockfd);
	if(ret == CHANDLER_OPERATION_FAILED)
		printf("%s : No such directory found",cmd->args[0]);
	return (ret);
}

int clientListDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	const BUF_SIZE=1024;
	char dir[BUF_SIZE];
	DIR* d=NULL;
	struct dirent* ent=NULL;
	cmd->cmd[0]=cmd->cmd[0];
	getcwd(dir,BUF_SIZE);
	d=opendir(dir);
	fts->mode=fts->mode;
	while((ent=readdir(d)) != NULL)
	{
		printf("%s\n",ent->d_name);
	}
	return (CHANDLER_SUCCESSFUL);
}

int serverListDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	sendCommand(cmd,fts->sockfd);
	printResponseText(fts->sockfd);
	return (CHANDLER_SUCCESSFUL);
}

int clientMakeDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	const int PERMS=0744;
	fts->mode=fts->mode;
	if(cmd->numArgs != 1)
	{
		puts("Incorrect number of arguments. Type 'help' for more details");
		return (CHANDLER_INCORRECT_NO_OF_ARGUMENTS);
	}
	if(mkdir(cmd->args[0],PERMS) < 0)
	{
		perror("Make directory");
		return (CHANDLER_OPERATION_FAILED);
	}
	return (CHANDLER_SUCCESSFUL);
}

int serverMakeDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	if(cmd->numArgs != 1)
	{
		puts("Incorrect number of arguments. Type 'help' for more details");
		return (CHANDLER_INCORRECT_NO_OF_ARGUMENTS);
	}
	sendCommand(cmd,fts->sockfd);
	printResponseText(fts->sockfd);
	return (receiveResponseCode(fts->sockfd));
}

int clientPrintWorkingDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	const BUF_SIZE=1024;
	char dir[BUF_SIZE];
	fts->mode=fts->mode;
	cmd->cmd[0]=cmd->cmd[0];
	getcwd(dir,BUF_SIZE);
	printf("%s\n",dir);
	return (CHANDLER_SUCCESSFUL);
}

int serverPrintWorkingDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	sendCommand(cmd,fts->sockfd);
	printResponseText(fts->sockfd);
	return (CHANDLER_SUCCESSFUL);
}

int GetFile(struct COMMAND* cmd,struct FTS* fts)
{
	FILE* df=NULL;
	const int ONEARG=1;
	const int TWOARGS=2;
	const int MAX=100;
	char* dfile;
	int ret;
	if(cmd->numArgs != ONEARG && cmd->numArgs != TWOARGS)
	{
		puts("Incorrect number of arguments. Type 'help' for more details");
		return (CHANDLER_INCORRECT_NO_OF_ARGUMENTS);
	}
	switch(cmd->numArgs)
	{
	case 1:
		dfile=cmd->args[0];
		if(fts->mode==FTS_MODE_ASCII)
			df=fopen(cmd->args[0],"w");
		else if(fts->mode == FTS_MODE_BINARY)
			df=fopen(cmd->args[0],"wb");
		break;
	case 2:
		dfile=cmd->args[1];
		if(fts->mode==FTS_MODE_ASCII)
			df=fopen(cmd->args[1],"w");
		else if(fts->mode == FTS_MODE_BINARY)
			df=fopen(cmd->args[1],"wb");
		break;
	}
	if(df==NULL)
	{
		printf("%s : Coulnd't open file for writing\n",dfile);
		return (CHANDLER_OPERATION_FAILED);
	}
	sendCommand(cmd,fts->sockfd);
	ret=receiveResponseCode(fts->sockfd);
	if(ret==0)	// File ready to be sent
	{
		if(fts->mode==FTS_MODE_ASCII)
		{
			writeAsciiResponseToFile(fts->sockfd,df);
		}
		else if(fts->mode==FTS_MODE_BINARY)
		{
			writeBinaryResponseToFile(fts->sockfd,df);
		}
		fclose(df);
	}
	else	// Some error has occurred
	{
		printResponseText(fts->sockfd);
		fclose(df);
		return (ret);
	}
	return (CHANDLER_SUCCESSFUL);
}

int MGetFile(struct COMMAND* cmd,struct FTS* fts)
{
	FILE* df=NULL;
	char c[2];
	int ret,i;
	if(cmd->numArgs < 1)
	{
		puts("No files specified. Type 'help' for more details");
		return (CHANDLER_INCORRECT_NO_OF_ARGUMENTS);
	}
	sendCommand(cmd,fts->sockfd);
	destroyCommandObject (cmd);
	receiveCommand (cmd,fts);
	for(i=0;i<cmd->numArgs;i++)
	{
		printf("%s : Do you copy this file to the current directory from the remote server? (y/n): ",cmd->args[i]);
		scanf(" \n%c",c);
		if(c[0]=='n')
		{
			printf("File '%s' skipped\n",cmd->args[i]);
			continue;
		}
		if(fts->mode==FTS_MODE_ASCII)
			df=fopen(cmd->args[i],"w");
		else if(fts->mode == FTS_MODE_BINARY)
			df=fopen(cmd->args[i],"wb");
		if(df==NULL)
		{
			printf("%s : Coulnd't open file for writing\n",cmd->args[i]);
			perror(cmd->args[i]);
			continue;
		}
		sendResponseCode(fts->sockfd,i);
		ret=receiveResponseCode(fts->sockfd);
		if(ret==0)	// File ready to be sent
		{
			if(fts->mode==FTS_MODE_ASCII)
			{
				writeAsciiResponseToFile(fts->sockfd,df);
			}
			else if(fts->mode==FTS_MODE_BINARY)
			{
				writeBinaryResponseToFile(fts->sockfd,df);
			}
			fclose(df);
		}
		else	// Some error has occurred
		{
			printResponseText(fts->sockfd);
			fclose(df);
		}
	}
	sendResponseCode(fts->sockfd,-1);
	return (CHANDLER_SUCCESSFUL);
}

int PutFile(struct COMMAND* cmd,struct FTS* fts)
{
	FILE* df=NULL;
	char* dfile;
	const int ONEARG=1;
	const int TWOARGS=2;
	int ret;
	const int BUF_SIZE=1024;
	if(cmd->numArgs != ONEARG && cmd->numArgs != TWOARGS)
	{
		puts("Incorrect number of arguments. Type 'help' for more details");
		return (CHANDLER_INCORRECT_NO_OF_ARGUMENTS);
	}
	sendCommand(cmd,fts->sockfd);
	ret=receiveResponseCode(fts->sockfd);
	if(ret==-1)
	{
		printResponseText(fts->sockfd);
		ret=receiveResponseCode(fts->sockfd);
		return (ret);
	}
	dfile=cmd->args[0];
	if(fts->mode==FTS_MODE_ASCII)
		df=fopen(cmd->args[0],"r");
	else if(fts->mode == FTS_MODE_BINARY)
		df=fopen(cmd->args[0],"rb");

	if(df==NULL)
	{
		char buf[BUF_SIZE];
		sendResponseCode(fts->sockfd,CHANDLER_OPERATION_FAILED);
		snprintf(buf,BUF_SIZE,"%s : Coulnd't open file for writing\n",dfile);
		write(fts->sockfd,buf,strlen(buf));
		writeEOFToSocket(fts->sockfd);
		return (CHANDLER_OPERATION_FAILED);
	}
	sendResponseCode(fts->sockfd,0);	// Sending success response code
	if(fts->mode==FTS_MODE_ASCII)
	{
		sendFileInAsciiMode(fts->sockfd,df);
	}
	else if(fts->mode==FTS_MODE_BINARY)
	{
		sendFileInBinaryMode(fts->sockfd,df);
	}
	return (CHANDLER_SUCCESSFUL);
}

int MPutFile(struct COMMAND* cmd,struct FTS* fts)
{
	FILE* df=NULL;
	int ret;
	char c[2];
	int i;
	if(cmd->numArgs < 1)
	{
		puts("No files specified. Type 'help' for more details");
		return (CHANDLER_INCORRECT_NO_OF_ARGUMENTS);
	}
	expandCommand (cmd);
	sendCommand(cmd,fts->sockfd);
	while(1)
	{
		i=receiveResponseCode(fts->sockfd);
		if(i == -1)
			break;
		printf("%s : Do you copy this file to the current directory from the remote server? (y/n): ",cmd->args[i]);
		scanf(" \n%c",c);
		if(c[0]=='n')
		{
			printf("File '%s' skipped\n",cmd->args[i]);
			sendResponseCode(fts->sockfd,-1);
			continue;
		}
		sendResponseCode(fts->sockfd,0);
		ret=receiveResponseCode(fts->sockfd);
		if(ret==-1)
		{
			printResponseText(fts->sockfd);
			continue;
		}
		if(fts->mode==FTS_MODE_ASCII)
			df=fopen(cmd->args[i],"r");
		else if(fts->mode == FTS_MODE_BINARY)
			df=fopen(cmd->args[i],"rb");
		ret=errno;
		if(df==NULL)
		{
			printf("%s : %s\n",cmd->args[i],strerror(ret));
			sendResponseCode(fts->sockfd,-1);
			continue;
		}
		sendResponseCode(fts->sockfd,0);
		if(fts->mode==FTS_MODE_ASCII)
		{
			sendFileInAsciiMode(fts->sockfd,df);
		}
		else if(fts->mode==FTS_MODE_BINARY)
		{
			sendFileInBinaryMode(fts->sockfd,df);
		}
		fclose(df);
	}
	return (CHANDLER_SUCCESSFUL);
}
