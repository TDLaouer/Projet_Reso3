#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "api.h"
#include "httpparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "request.h"

char* Semantique(Lnode * root);

int CountToken (_Token * list);

int VerifMethod ( char* answer);

int VerifTarget ( char* answer);

int VerifHTTPVersion ( char* answer);

int VerifHost (char* answer);

int verifAccept ( char* answer);

int verifAcceptEncoding (char* answer);

int verifUserAgent ( char* answer);

int verifTransferEncoding (char* answer);

int VerifCookie (char* answer);

int VerifReferer (char* answer);

int VerifContentLength (char* answer);

int VerifConnection (char* answer);



#endif
