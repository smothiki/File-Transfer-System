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

#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<strings.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<pthread.h>
#include "logger.h"
#include "authenticator.h"
#include "logger.h"
#include "config_reader.h"
#define MSGSIZE 100
struct msgbuf
{
        long mtype;
        char mtext[MSGSIZE];
};
int sockfd,newsockfd;
struct LOGGER *llg;
void term_handle(int x)
{
	close(sockfd);
	close(newsockfd);
	destroyLogger(llg);
	printf("closing server: %d",x);
	exit(0);
}
void* logger(void* inp);
int main(int argc,char *argv[])
{
	const int BUF_SIZE=1024;
	struct sockaddr_in server_address,client_address;
	int server_length,client_length;
	char buf[BUF_SIZE];
	short port=atoi(argv[1]);
	int n;
	pthread_t pt;
	struct CONFIG_READER cfgr;
	int num_log_files;
	int log_file_size;
	char uname[BUF_SIZE];
	char home_dir[BUF_SIZE];
	struct LOGGER lg;
	llg=&lg;
	createConfigReader(&cfgr,"config.cfg");
	getConfigValue("NLOGFILES",buf,&cfgr);
	num_log_files=atoi(buf);
	getConfigValue("LOGFILESIZE",buf,&cfgr);
	log_file_size=atoi(buf);
	//printf("vals: %d %d\n",num_log_files,log_file_size);
	createLogger(&lg,"fts_server_log","server_logs",num_log_files,log_file_size);
	pthread_create(&pt,NULL,logger,NULL);
	signal(SIGTERM,term_handle);
	bzero((char *)&server_address,sizeof(server_address));
	bzero((char *)&client_address,sizeof(client_address));
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=INADDR_ANY;
	server_address.sin_port=htons(port);
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("in socket creation");
		exit(1);
	}
	server_length=client_length=(int)sizeof(struct sockaddr_in);
	if(bind(sockfd,(struct sockaddr *)&server_address,server_length) < 0)
	{
		perror("in socket binding");
		exit(1);
	}
	int a=5;
	listen(sockfd,a);
	printf("server started at port: %d\n",port);
	while(1)
	{
		//printf("before accept\n");
		newsockfd=accept(sockfd,(struct sockaddr *)&client_address,&client_length);
		//printf("sockfd: %d\n",newsockfd);
		if(newsockfd < 0)
		{
			perror("fail at accept");
			close(sockfd);
			exit(1);
		}
		{
			int n;
			n=read(newsockfd,uname,sizeof(uname));
			//printf("num: %d\n",n);
			uname[n]='\0';
			//printf("unam: %s\n",uname);
			if(authenticateUser(uname,home_dir) != AUTHENTICATOR_SUCCESSFUL)
			{
				int len=-1;
				write(newsockfd,&len,sizeof(len));
				close(newsockfd);
				continue;
			}
			int len=0;
			write(newsockfd,&len,sizeof(len));
		}
		if(!fork())
		{
			const int ten=10;
			char dir[BUF_SIZE];
			char sfd[ten];
			getcwd(dir,BUF_SIZE);
			strncat(dir,"/fts_server",BUF_SIZE);
			snprintf(sfd,BUF_SIZE,"%d",newsockfd);
			chdir(home_dir);
			//printf("spawning handler");
			execl(dir,"fts_server",sfd,(char *)0);
			perror("execl");
			exit(1);
		}
		close(newsockfd);
	}
	close(sockfd);
	close(newsockfd);
	return (0);
}


void* logger(void* inp)
{
	const int KEYGENNUM=9;
	const int PERMS=0600;
	key_t x=ftok("/etc/passwd",KEYGENNUM);
	const int DELAY=2;
	struct msgbuf mb;
	int mid=msgget(x,IPC_CREAT | PERMS),ret;
	//printf("mid: %d\n",mid);
	struct msqid_ds mds;
	msgctl(mid,IPC_STAT,&mds);
	inp=inp;
	while(1)
	{
		if(mds.msg_qnum > 0)
		{
			ret=msgrcv(mid,&mb,sizeof(mb.mtext),1,0);
			//printf("message: %s\n",mb.mtext);
			writeToLogFile(mb.mtext,llg);
		}
		else
			sleep(DELAY);
		msgctl(mid,IPC_STAT,&mds);
	}
	return ((void*)0);
}
