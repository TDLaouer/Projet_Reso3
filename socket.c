#include "socket.h"


char* liresocket(int fd){

	int r;
	char* message = malloc(SUPERMAX*sizeof(char));
	int vraie_taille=0;
	int w=0;
	char headertruc[8];
	while (w<8)
	{
		w+= read(fd, headertruc, 8);
	}
	vraie_taille = headertruc[4]*16 + headertruc[5]  + headertruc[6];
	while (vraie_taille>0){
		r = read(fd, message, vraie_taille);
		vraie_taille -=r;
	}
	strncat(message, "\0", 1);
	printf("Message retourné : '%s'\n", message);
	return message;
}

void sendpara(int fd, unsigned short requestID,char* script){
	FCGI_Header h;
	h.version=FCGI_VERSION_1;
	h.type=FCGI_PARAMS;
	h.requestId=1   /*htons(requestID)*/;
	h.contentLength=0;
	h.paddingLength=0;
	addNameValuePair(&h,"PATH",script);
	addNameValuePair(&h,"FILE")
	addNameValuePair(&h,"REQUEST_METHOD","GET");

	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
}



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

	return 0;
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
#define sendStdin(fd,id,stdin,len) sendWebData(fd,FCGI_STDIN,id,stdin,len)
#define sendData(fd,id,data,len) sendWebData(fd,FCGI_DATA,id,data,len)
//============================================================================================================ //

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
	//int enable = 1;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket creation failed\n");
		return (-1);
	}

	bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	inet_aton("127.0.0.1",serv_addr.sin_addr.s_addr);
	serv_addr.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect failed\n");
		return (-1);
	}

	return fd;
}

// =========================================================================================================== //
char* PHP(char* p, int method)
{
	int fd;
	_Token* rbody; Lnode* n, *root;
	fd=createSocket(9000);
	sendGetValue(fd);
	sendBeginRequest(fd,1,FCGI_RESPONDER,FCGI_KEEP_CONN);
	sendpara(fd,1,p);
	root = getRootTree();
	rbody = searchTree(root,"message_body");
	n=rbody->node;
	if (method ==1){ /*GET*/
		sendStdin(fd,1,n->value,n->len);
	}else{ /*Autre*/
		sendStdin(fd,1,NULL,0);
	}
	sleep(1);
	return liresocket(fd);
}
