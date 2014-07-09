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
#include<sys/ioctl.h>
#include<string.h>
void displayProgressBar(int x)
{
	const int hun=100;
	int n;
	int i;
	char cr='\r';
	int numcols;
	struct winsize w;
	const char* ending="(COMPLETED)";
	const int EXTRA_CHARS=8;
	int barlen;
	x=(x>hun) ? x%hun : x;
	ioctl(0, TIOCGWINSZ, &w);
	numcols=w.ws_col;
	barlen=(numcols  - strlen(ending) - EXTRA_CHARS);
	n= barlen * x /hun;
	printf("%c",cr);
	printf("%3d\% [",x);
	fflush(stdout);
	for(i=0;i<n;i++)
	{
		printf("%s","=");
	}
	for(;i<barlen;i++)
		printf("%s"," ");
	printf("%s","]");
	if(x == hun)
	{
		printf(" %s",ending);
	}
	fflush(stdout);
}
