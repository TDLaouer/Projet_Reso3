#ifndef SEMANTIC_H
#define SEMANTIC_H


char* Semantique(Node * root);

int CountToken ( _Token * list);

void VerifMethod ( _Token * list);

void VerifTarget ( _Token * list);

void VerifHTTPVersion ( _Token * list);

void VerifHost ( _Token * list);

void verifAccept ( _Token * list);

void verifAcceptEncoding ( _Token * list);

void verifUserAgent ( _Token * list);

void verifTransferEncoding ( _Token * list);

void VerifCookie ( _Token * list);

void VerifReferer ( _Token * list);

void VerifContentLength ( _Token * list);

void VerifConnection ( _Token * list);



#endif
