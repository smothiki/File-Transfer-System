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
#include "server_cmd_handlers.h"
#include "command_interpreter.h"
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>


int receiveCommand(struct COMMAND* cmd,struct FTS* fts)
{
	const int BUF_SIZE=1024;
	char buf[BUF_SIZE];
	int n;
	n=read(fts->sockfd,buf,BUF_SIZE);
	buf[n]='\0';
	fillCommandObject(buf,cmd);
	return (0);
}

int sendResponseCode(int sockfd,int code)
{
	write(sockfd,&code,sizeof(code));
	return (0);
}

int printResponseText(int sockfd)
{
	const int BUF_SIZE=1024;
	char buf[BUF_SIZE];
	int n;
	while((n=read(sockfd,buf,1)) > 0)
	{
		if(buf[0]=='\0')
			break;
		else
			putchar(buf[0]);
	}
	return (0);
}

int writeEOFToSocket(int sockfd)
{
	char c='\0';
	write(sockfd,&c,1);
	return (0);
}

int sendFileInBinaryMode(int sockfd,FILE* f)
{
	const int BUF_SIZE=1024;
	unsigned long len;
	char buf[BUF_SIZE];
	int i,n;
	fseek(f,0,SEEK_END);
	len=ftell(f);
	//printf("length: %lu\n",len);
	write(sockfd,&len,sizeof(len));
	fseek(f,0,SEEK_SET);
	for(i=0;i<len;)
	{
		n=fread(buf,BUF_SIZE,1,f);
		write(sockfd,buf,ftell(f)-i);
		i=ftell(f);
		//printf("i val: %d %d\n",i,n);
	}
	return (0);
}

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
	struct dirent* ent;
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
		return (CHANDLER_SUCCESSFUL);
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
	arr=(char **)malloc(sizeof(char *) * newcount);
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
	return (CHANDLER_SUCCESSFUL);
}

int sendFileInAsciiMode(int sockfd,FILE* f)
{
	int buf;
	char c;
	int len;
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
			write(sockfd,&c,1);
	}
	writeEOFToSocket(sockfd);
	return (CHANDLER_SUCCESSFUL);
}

int writeAsciiResponseToFile(int sockfd,FILE* f)
{
	char buf[2];
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
			//displayProgressBar(count / (double)len * 100.0);
		}
	}
	//printf("\n");
	return (CHANDLER_SUCCESSFUL);
}

int writeBinaryResponseToFile(int sockfd,FILE* f)
{
	unsigned long len;
	int n,i;
	const int BUF_SIZE=1024;
	char buf[BUF_SIZE];
	read(sockfd,&len,sizeof(len));
	printf("length: %lu\n",len);
	for(i=0;i<len;)
	{
		n=read(sockfd,buf,BUF_SIZE);
		fwrite(buf,n,1,f);
		i+=n;
		//displayProgressBar(i / (double)len * 100.0);
	}
	//printf("\n");
	return (0);
}

int receiveResponseCode(int sockfd)
{
	int x,ret;
	ret=read(sockfd,&x,sizeof(x));
	return ((ret>0)?x:-1);
}

int ChangeMode(struct COMMAND* cmd,struct FTS* fts)
{
	if(!strcmp(cmd->cmd,"ascii"))
	{
		fts->mode=FTS_MODE_ASCII;
		sendResponseCode(fts->sockfd,CHANDLER_SUCCESSFUL);
		return (CHANDLER_SUCCESSFUL);
	}
	else if(!strcmp(cmd->cmd,"bin"))
	{
		fts->mode=FTS_MODE_BINARY;
		sendResponseCode(fts->sockfd,CHANDLER_SUCCESSFUL);
		return (CHANDLER_SUCCESSFUL);
	}
	sendResponseCode(fts->sockfd,CHANDLER_UNEXPECTED_ERROR);
	return (CHANDLER_UNEXPECTED_ERROR);
}

int exitProgram(struct COMMAND* cmd,struct FTS* fts)
{
	fts->mode=fts->mode;
	cmd->cmd[0]=cmd->cmd[0];
	exit(0);
}


int serverChangeDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	if(chdir(cmd->args[0]) < 0)
	{
		sendResponseCode(fts->sockfd,CHANDLER_OPERATION_FAILED);
		return (CHANDLER_OPERATION_FAILED);
	}
	else
	{
		sendResponseCode(fts->sockfd,CHANDLER_SUCCESSFUL);
		return (CHANDLER_SUCCESSFUL);
	}
}

int serverListDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	const int BUF_SIZE=1024;
	char dir[BUF_SIZE],buf[BUF_SIZE];
	DIR* d=NULL;
	struct dirent* ent=NULL;
	fts->mode=fts->mode;
	cmd->cmd[0]=cmd->cmd[0];
	getcwd(dir,BUF_SIZE);
	d=opendir(dir);
	while((ent=readdir(d)) != NULL)
	{
		snprintf(buf,BUF_SIZE,"%s\n",ent->d_name);
		write(fts->sockfd,buf,strlen(buf));
	}
	writeEOFToSocket(fts->sockfd);
	return (CHANDLER_SUCCESSFUL);
}

int serverMakeDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	const int BUF_SIZE=1024;
	char buf[BUF_SIZE];
	const int PERMS=0744;
	if(mkdir(cmd->args[0],PERMS) < 0)
	{
		snprintf(buf,BUF_SIZE,"Make directory : %s\n",strerror(errno));
		write(fts->sockfd,buf,strlen(buf));
		writeEOFToSocket(fts->sockfd);
		sendResponseCode(fts->sockfd,CHANDLER_OPERATION_FAILED);
		return (CHANDLER_OPERATION_FAILED);
	}
	writeEOFToSocket(fts->sockfd);
	sendResponseCode(fts->sockfd,CHANDLER_SUCCESSFUL);
	return (CHANDLER_SUCCESSFUL);
}

int serverPrintWorkingDirectory(struct COMMAND* cmd,struct FTS* fts)
{
	const int BUF_SIZE=1024;
	char dir[BUF_SIZE],buf[BUF_SIZE];
	printf("at start\n");
	fts->mode=fts->mode;
	getcwd(dir,BUF_SIZE);
	cmd->cmd[0]=cmd->cmd[0];
	snprintf(buf,BUF_SIZE,"%s\n",dir);
	write(fts->sockfd,buf,strlen(buf));
	writeEOFToSocket(fts->sockfd);
	printf("end of serverpwd\n");
	return (CHANDLER_SUCCESSFUL);
}

int GetFile(struct COMMAND* cmd,struct FTS* fts)
{
	const int BUF_SIZE=1024;
	FILE* df=NULL;
	char* dfile;

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

int MGetFile(struct COMMAND* cmd,struct FTS* fts)
{
	const int BUF_SIZE=1024;
	FILE* df=NULL;
	int code;
	puts("before expanding");
	expandCommand(cmd);
	puts ("after expanding");
	sendCommand (cmd,fts->sockfd);
	while(1)
	{
		code=receiveResponseCode(fts->sockfd);
		if(code == -1)
			break;
		if(fts->mode==FTS_MODE_ASCII)
			df=fopen(cmd->args[code],"r");
		else if(fts->mode == FTS_MODE_BINARY)
			df=fopen(cmd->args[code],"rb");
		if(df==NULL)
		{
			char buf[BUF_SIZE];
			snprintf(buf,BUF_SIZE,"%s : Coulnd't open file for writing\n",cmd->args[code]);
			sendResponseCode(fts->sockfd,-1);
			write(fts->sockfd,buf,strlen(buf));
			writeEOFToSocket(fts->sockfd);
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

int PutFile(struct COMMAND* cmd,struct FTS* fts)
{
	const int BUF_SIZE=1024;
	FILE* df=NULL;
	char* dfile;
	int ret;
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
		const int BUF_SIZE=1024;
		char buf[BUF_SIZE];
		sendResponseCode (fts->sockfd,-1);
		snprintf(buf,BUF_SIZE,"%s : Coulnd't open file for writing\n",dfile);
		write(fts->sockfd,buf,strlen(buf));
		writeEOFToSocket (fts->sockfd);
		sendResponseCode (fts->sockfd,CHANDLER_OPERATION_FAILED);
		return (CHANDLER_OPERATION_FAILED);
	}
	sendResponseCode (fts->sockfd,0);
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

int MPutFile(struct COMMAND* cmd,struct FTS* fts)
{
	const int BUF_SIZE=1024;
	FILE* df=NULL;
	int ret,i;
	for(i=0;i<cmd->numArgs;i++)
	{
		sendResponseCode (fts->sockfd,i);
		ret=receiveResponseCode (fts->sockfd);
		if(ret==-1)
			continue;
		if(fts->mode==FTS_MODE_ASCII)
			df=fopen(cmd->args[i],"w");
		else if(fts->mode == FTS_MODE_BINARY)
			df=fopen(cmd->args[i],"wb");
		ret=errno;
		if(df==NULL)
		{
			char buf[BUF_SIZE];
			sendResponseCode (fts->sockfd,-1);
			snprintf(buf,BUF_SIZE,"%s : %s\n",cmd->args[i],strerror(ret));
			continue;
		}
		sendResponseCode(fts->sockfd,0);
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
			fclose(df);
		}
	}
	sendResponseCode(fts->sockfd,-1);
	return (CHANDLER_SUCCESSFUL);
}
