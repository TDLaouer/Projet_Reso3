#ifndef SEMANTIC_H
#define SEMANTIC_H
#define MAX 15000
#define SUPERMAX 1500000
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "semantic.h"
// this will declare internal type used by the parser
#include "httpparser.h"
#include "api.h"

int BigHost(char* p);
int Connection(char *p );
int cookie(char*p);
int TransferEnco(char *p);
int contentlenght(char* p);
int method(char *p);
int version2(char *p);
int version1(char *p);
int version(char *p);
int host(char* p);
int Connection2(char *p );
int originform(char* p);
int Verification();
int mediarange(char* p, char* monfichier);
int lecturefich(char *monFichier, char* montype, int len);
int co(char*);
char* returnInChar(int val);

#endif
