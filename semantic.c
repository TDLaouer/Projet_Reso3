
#include "semantic.h"

// Connection("connection-option")
int Connection(char *p ){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	if (p != NULL) {
		r=searchTree(root,p);
		tok=r;
		while(tok){

		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
		if( strcmp(node->value ,"Connection") == 0 || strcmp( node->value , "Host")==0 || strcmp(node->value , "Content-Length")==0 || strcmp(node->value , "Accept-Encoding")==0 ||  strcmp(node->value , "Accept")==0 || strcmp(node->value , "User-Agent") ==0 || strcmp(node->value , "Referer")==0 || strcmp(node->value , "Transfer-Encoding")==0 || strcmp(node->value , "Cookie")==0){
			return(400);
		}
		tok=tok->next;
		}
		
	}
return(200);
}



// Connection2("connection-option")
int Connection2(char *p ){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	if (p != NULL) {
		
		r=searchTree(root,p);
		tok=r;
		while(tok){
		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
		if(strcmp(node->value , "close") == 0){
			return(1);
		}
		tok=tok->next;
		}
		/*purgeElement(&r);
		purgeTree(root);*/
	}
return(200);
}




int BigHost(char* p){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	if (p != NULL) {
		r=searchTree(root,p);
		if(r==NULL){return(0);}
		tok=r;
		printf("coucu\n");
		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
/*		purgeElement(&r);
		purgeTree(root);*/
	}
return(200);
}

int co(char* p){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	if (p != NULL) {
		r=searchTree(root,p);
		tok=r;

		node=(Lnode *)tok->node;
		if(tok->next!=NULL)return(400);
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
	}
return(200);
}


int host(char* p){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	if (p != NULL) {
		r=searchTree(root,p);
		
		if(r==NULL){printf("cpiji\n");return(0);}
		tok=r;
		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);

	}
	return(200);
}


int originform(char* p){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	
	if (p != NULL) {
		r=searchTree(root,p);
		
		if(r==NULL)return(501);
		tok=r;
		
		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
	}
	return(200);
}


/* à mettre dans le main: version("HTTP-name") version1("MAJ") version2("MIN")*/
int version(char *p){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	if (p != NULL) {
		r=searchTree(root,p);
		tok=r;
		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);

	/*purgeElement(&r);
		purgeTree(root);*/
		if(tok->next!=NULL)return(400);
		if( strcmp(node->value,"HTTP") != 0)return(400);
	}
	return(200);
}

int version1(char *p){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	if (p != NULL) {
		r=searchTree(root,p);
		
		tok=r;
		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
		if(tok->next!=NULL)return(400);
		/*purgeElement(&r);
		purgeTree(root);*/
	}
if( strncmp(node->value,"1",1) != 0)return(505);
return(200);
}
int version2(char *p){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	int ra;
	int val;
	if (p != NULL) {
		r=searchTree(root,p);
		tok=r;

		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
		if(tok->next!=NULL)return(400);
	}

	if( strncmp(node->value,"HTTP/1.1",8) != 0 && strncmp(node->value,"HTTP/1.0",8)!=0)return(505);
	if(strncmp(node->value,"HTTP/1.1",8)==0){
		ra=BigHost("Host");
		
		if(ra==0)return(400);
		val=host("IP_litteral");
		if(val!=0)return(501);
	}
	if(strncmp(node->value,"HTTP/1.0",8)==0){
		ra=BigHost("Host");
		//printf("%d\n",ra);
		if(ra!=0){
		val=host("IP_litteral");
		if(val!=0)  return(501);
		}

	}
return(200);
}

// method("method");
int method(char *p){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	if (p != NULL) {
		r=searchTree(root,p);
		tok=r;
		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
		if(tok->next!=NULL)return(400);
		
if(strncmp(node->value,"GET",3)!=0 && strncmp(node->value, "HEAD",4)!=0 && strncmp(node->value , "POST", 4)!=0){return(501);}

}

return(200);
}

//contentlenght("Content-Length-header",val)
int contentlenght(char* p){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	if (p != NULL) {
		r=searchTree(root,p);
		if(r==NULL){return(1);}
		tok=r;

		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
		if(tok->next!=NULL)return(400);
		
		
	}
	if(TransferEnco("Transfer_Encoding") == 1){return(400);}
return(200);
}

int TransferEnco(char *p){
	_Token *r,*tok;
	Lnode *root;Lnode *node;
	root=getRootTree();
	char coucou[MAX];
	int i=1;

	if (p != NULL) {
		r=searchTree(root,p);
		if(r==NULL){return(1);}
		tok=r;
		if(tok->next!=NULL)return(400);
		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
		char* mem=strchr(node->value,',');
		while(  strchr(mem,',') != NULL){
			mem=strchr(mem,',');
		}
		while( mem[i] != '\0' ){
			coucou[i-1]=mem[i];
		}
		if(strcmp(coucou , " chunked") != 0){
			if( Connection2("connection_option") != 1){
			return(400);
			}
		}
		if(contentlenght("Content_length") == 1){return(400);}
		/*purgeElement(&r);
		purgeTree(root);*/
	}
return(200);
}

// cookie("Cookie")
int cookie(char*p){
	_Token *r,*tok; Lnode *node;
	Lnode *root;
	root=getRootTree();
	if (p != NULL) {
		r=searchTree(root,p);
		if(r==NULL)return(200);
		tok=r;
		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);

}
if(tok->next!=NULL)return(400);

return(200);
}


int lecturefich(char *monFichier, char* montype){
	FILE* fichier;
	char chaine[MAX]="";
	fichier=fopen(monFichier,"r");
	if(fichier==NULL)
	{
		perror("Probleme ouverture fichier");
		exit(1);
	}
	while(fgets(chaine, MAX, fichier) != NULL)
	{
		if(strcmp(montype,chaine) == 0){
			return(1);
		}

	}
	fclose(fichier);
	return (-1);

}


int mediarange(char* p, char* monfichier){
	_Token *r,*tok; Lnode *node;
	Lnode *root;
	root=getRootTree();
	int oui=0;
	int ra;
	if (p != NULL) {
		r=searchTree(root,p);
		tok=r;
		while(tok){
		node=(Lnode *)tok->node;
		printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
		while(node->value[oui] != ';' || oui< node->len )oui++;

		if(oui<node->len)node->value[oui]='\0';
		ra=lecturefich(monfichier, node->value);
		if(ra==-1)return(400);
		tok=tok->next;
		}
		/*purgeElement(&r);
		purgeTree(root);*/
	}

return(200);
}
int Verification(){
	int ver;
	int conn=200;
	if( (ver=method("method")) != 200  ) return(ver);
	
	if( (ver=originform("origin_form")) != 200  ) return(ver);
	

	if( (ver=version2("HTTP_version")) != 200  ) return(ver);
	printf("coidifnze\n");
	if( (ver=cookie("Cookie")) != 200  ) return(ver);
	printf("testé");
	if( (ver=contentlenght("Content_Length")) != 200 && (ver!=1)){return(conn);}
	if( (conn=TransferEnco("Transfer_Encoding")) != 200  &&  (conn!=1)){return(ver);}
	if( (ver=mediarange("media_range","accept-types.txt") !=200 ))return(ver);
	/* Test si les header ne sont pas en double >>> Bad Request*/
	if( (ver=co("Connection") != 200)) return(ver);
	if( (ver=co("Referer") != 200)) return(ver);
	if( (ver=co("User_Agent") != 200)) return(ver);
	if( (ver=co("Host") != 200)) return(ver);
	if( (ver=co("Accept") != 200)) return(ver);
	if( (ver=co("Accept_Encoding") != 200)) return(ver);
	return(conn);
}


char * returnInChar(int val){
	printf("%d\n",val);
	if (val == 501){
		return "HTTP/1.1 501 Not Implemented\0";
	}else if (val == 505){
		return "HTTP/1.1 505 HTTP Version Not Supported\0";
	}else if (val == 200){ // good, ok -> conncection keep-alive
		return "HTTP/1.1 200 OK\r\nConnection: keep-alive\0";
	}else if (val == 1){ // good, ok -> connection close
		return "HTTP/1.1 200 OK\r\nConnection: close\0";
	}else{
		return "HTTP/1.1 400 Bad Request\0";
	}
	
}
