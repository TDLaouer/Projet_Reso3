#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include "fastcgi.h"
#include "semantic.h"

void writeSocket(int fd,FCGI_Header *h,unsigned int len);

void writeLen(int len, char **p) ;

int addNameValuePair(FCGI_Header *h,char *name,char *value);

void sendGetValue(int fd);

void sendBeginRequest(int fd,unsigned short requestId,unsigned short role,unsigned char flags);

void sendAbortRequest(int fd,unsigned short requestId);

void sendWebData(int fd,unsigned char type,unsigned short requestId,char *data,unsigned int len);

static int createSocket(int port);

char* liresocket(int fd);

void sendpara(int fd, unsigned short requestID,char* script);

char* PHP(char* p, int method);

#endif
