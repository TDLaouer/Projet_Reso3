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
#include "fastcgi.h"

// =========================================================================================================== //
void writeSocket(int fd,FCGI_Header *h,unsigned int len)
{
	int w;

	h->contentLength=htons(h->contentLength);
	h->paddingLength=htons(h->paddingLength);

	while (len) {
	// try to write
		do {
			w = write(fd, h, len);
		} while (w == -1 && errno == EINTR);
	len-=w;
	}
}

// =========================================================================================================== //
void readSocket(int fd,char* socket_output){
	int w=0;
	int sz;
	unsigned char fcgi_header_read[8];
	FILE* output = fopen(socket_output, "w");
	while(w<8){
		w += read(fd, fcgi_header_read, 8);
	}
	//printf("%d\n", fcgi_header_read[5]);
	sz = fcgi_header_read[4]*256+fcgi_header_read[5]+fcgi_header_read[6];
	printf("%d\n", sz);
	if(sz>0){
		char* r_socket = malloc(sz*sizeof(char));
		while(sz) {
			do {
				w = read(fd, r_socket, sz);
			} while(w==-1 && errno == EINTR);
			r_socket[w] = '\0';
			fputs(r_socket, output);
			sz-=w;
		}
	}
	fclose(output);
}

// =========================================================================================================== //
void writeLen(int len, char **p) {
	if (len > 0x7F ) {
		*((*p)++)=(len>>24)&0x7F;
		*((*p)++)=(len>>16)&0xFF;
		*((*p)++)=(len>>8)&0xFF;
		*((*p)++)=(len)&0xFF;
	} else *((*p)++)=(len)&0x7F;
}

// =========================================================================================================== //
int addNameValuePair(FCGI_Header *h,char *name,char *value)
{
	char *p;
	unsigned int nameLen=0,valueLen=0;

	if (name) nameLen=strlen(name);
	if (value) valueLen=strlen(value);

	if ((valueLen > FASTCGIMAXNVPAIR) || (valueLen > FASTCGIMAXNVPAIR) ) return -1;
	if ((h->contentLength+((nameLen>0x7F)?4:1)+((valueLen>0x7F)?4:1)) > FASTCGILENGTH ) return -1;

	p=(h->contentData)+h->contentLength;
	writeLen(nameLen,&p);
	writeLen(valueLen,&p);
	strncpy(p,name,nameLen);
	p+=nameLen;
	if (value) strncpy(p,value,valueLen);
	h->contentLength+=nameLen+((nameLen>0x7F)?4:1);
	h->contentLength+=valueLen+((valueLen>0x7F)?4:1);
	return(0);
}
// =========================================================================================================== //

void sendGetValue(int fd)
{
FCGI_Header h;

	h.version=FCGI_VERSION_1;
	h.type=FCGI_GET_VALUES;
	h.requestId=htons(FCGI_NULL_REQUEST_ID);
	h.contentLength=0;
	h.paddingLength=0;
	addNameValuePair(&h,FCGI_MAX_CONNS,NULL);
	addNameValuePair(&h,FCGI_MAX_REQS,NULL);
	addNameValuePair(&h,FCGI_MPXS_CONNS,NULL);
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
}
// =========================================================================================================== //
void sendBeginRequest(int fd,unsigned short requestId,unsigned short role,unsigned char flags)
{
FCGI_Header h;
FCGI_BeginRequestBody *begin;

	h.version=FCGI_VERSION_1;
	h.type=FCGI_BEGIN_REQUEST;
	h.requestId=htons(requestId);
	h.contentLength=sizeof(FCGI_BeginRequestBody);
	h.paddingLength=0;
	begin=(FCGI_BeginRequestBody *)&(h.contentData);
	begin->role=htons(role);
	begin->flags=flags;
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
}
// =========================================================================================================== //
void sendAbortRequest(int fd,unsigned short requestId)
{
FCGI_Header h;

	h.version=FCGI_VERSION_1;
	h.type=htons(FCGI_ABORT_REQUEST);
	h.requestId=requestId;
	h.contentLength=0;
	h.paddingLength=0;
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
}
//============================================================================================================ //
void sendParams(int fd, unsigned short requestId, char* script){
	FCGI_Header h;

	h.version=FCGI_VERSION_1;
	h.type=FCGI_PARAMS;
	h.requestId=htons(requestId);
	h.contentLength=0;
	h.paddingLength=0;
	addNameValuePair(&h,"SCRIPT_FILENAME",script);
	addNameValuePair(&h,"REQUEST_METHOD","GET");
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
}

#define sendStdin(fd,id,stdin,len) sendWebData(fd,FCGI_STDIN,id,stdin,len)
#define sendData(fd,id,data,len) sendWebData(fd,FCGI_DATA,id,data,len)
//============================================================================================================
void sendWebData(int fd,unsigned char type,unsigned short requestId,char *data,unsigned int len)
{
FCGI_Header h;

	if (len > FASTCGILENGTH) return ;

	h.version=FCGI_VERSION_1;
	h.type=type;
	h.requestId=htons(requestId);
	h.contentLength=len;
	h.paddingLength=0;
	memcpy(h.contentData,data,len);
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
}
// =========================================================================================================== //
static int createSocket(int port)
{
	int fd;
	struct sockaddr_in serv_addr;
	int enable = 1;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket creation failed\n");
		return (-1);
	}

	bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	inet_aton("127.0.0.1",&serv_addr.sin_addr);
	serv_addr.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect failed\n");
		return (-1);
	}
	return fd;
}
// =========================================================================================================== //

void FCGI_request(char* requested_file, int port){
	int fd = createSocket(port);
	//sendGetValue(fd);
	sendBeginRequest(fd,1,FCGI_RESPONDER,FCGI_KEEP_CONN);
	sendParams(fd,1,requested_file);
	sendStdin(fd,1,NULL,0);
	//sleep(2);
	readSocket(fd, "php_output");
}
