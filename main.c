#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "semantic.h"
#include "socket.h"

#define SUPERMAX 1500000

#define REPONSE "HTTP/1.0 200 OK\r\n\r\n"
#define ABSOLU "/home/userir/Téléchargements/Files"
#define ABSOLU_LEN 36
static long body_size;
static int php_used =0;


char* server_fileToSend(char* file,int len) {
	char* sendbody, *chemin = malloc(MAX*sizeof(char));
	_Token *r,*tok;
	Lnode * root=getRootTree(),*n;
	int i=0;

	// si les 3e derniers caractères de file sont "php", alors on fait le fastCGI

	if (strncmp(file + len - 4, ".php", 4) == 0) {
		FILE *f;
		char* pointeur=malloc(SUPERMAX*sizeof(char));
		r=searchTree(root,"method");
		n= r->node;
		strncpy(chemin, ABSOLU, ABSOLU_LEN);
		strncat(chemin+ABSOLU_LEN,file, len);
		if (strncmp(n->value, "GET", n->len)==0){
			pointeur = PHP(chemin, 1);
		}else {
			pointeur = PHP(chemin, 0);
		}
		strncat(pointeur, "\0", 1);

		php_used=1;
		body_size = strlen(pointeur);
		return pointeur;
	}


	char* filename=(char*)calloc(SUPERMAX,sizeof(char));
	r=searchTree(root,"Host");
	tok=r;
	if (tok!=NULL){
		n=(Lnode*) tok->node;
		/*strncpy(filename,n->value, n->len);
		i=n->len;*/
	}
	if (file[0]=='/'){
		if (i!=0){
			strncpy(filename+i,file,len);
		}
		else{
			strncpy(filename+i,file+1,len-1);
		}
	}
	else{
		strncpy(filename,file,len);
	}
	strncat(filename,"\0",1);
	//printf("filetruc : '%s'\n",filename);
	FILE* f = fopen(filename, "rb");
	if(f == NULL) {
		f = fopen("www/notfound.html", "rb");
	}
	fseek(f, 0, SEEK_END);
	body_size = ftell(f);
	printf("\nla taille du fichier %d\n", body_size);
	fseek(f, 0, SEEK_SET);

	if((sendbody = malloc(body_size + 1)) == NULL)
	{
		sendbody = malloc(20*sizeof(char));
		sendbody ="this is a directory\0";
		body_size=20;
		return sendbody;
	}

	else{
		fread(sendbody, body_size, 1, f);
		strcat(sendbody, "\0");
	}
	fclose(f);
	return sendbody;
}


int main(int argc, char *argv[])
{
	message *requete;
	message *reponse;
	char* sendbody = (char*)malloc(SUPERMAX*sizeof(char));
	char* target = (char*)malloc(SUPERMAX*sizeof(char));
	while ( 1 ) {
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest.
		if ((requete=getRequest(8080)) == NULL ) return -1;

		// Affichage de debug
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId);
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);
		int r = parseur(requete->buf, requete->len);
		char *rep = (char*)malloc(SUPERMAX*sizeof(char));
		strncpy(rep, returnInChar(Verification()), SUPERMAX);
		printf("reponse:%s\n",rep);

		if (strcmp(rep, "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n") == 0 || strcmp(rep, "HTTP/1.1 200 OK\r\nConnection: close\r\n") == 0)
		{
			_Token * tmp;
			char * bla=malloc(MAX*sizeof(char));
			tmp = searchTree(getRootTree(),"method");
			Lnode * r = (Lnode*)tmp->node;
			int* lenMethod = 0;
			char* method = getElementValue(tmp->node ,lenMethod);
			//printf("oui ? %s\n", method);
			if (strncmp(method, "GET",3) == 0)
			{
				_Token * tmp2;
				tmp2 = searchTree(getRootTree(), "request_target");
				Lnode* r2 = (Lnode*)tmp2->node;
				target = r2->value;
				//printf("%s\n", target);
				strcpy(sendbody, server_fileToSend(target,r2->len));
				printf("\n\nsendbody = %s\n", sendbody);
				memcpy(rep+strlen(rep), "Content-Length: ",16);

				sprintf(bla,"%d", body_size);

				memcpy(rep+strlen(rep), bla, strlen(bla));
				if (php_used)
				{
					php_used = 0;
					memcpy(rep+strlen(rep), "\r\n",2);
				}
				else memcpy(rep+strlen(rep), "\r\n\r\n",4);
				memcpy(rep+strlen(rep), sendbody, body_size);


			}
		}


		if ((reponse=malloc(sizeof(message))) != NULL ) {

			reponse->len=strlen(rep);
			printf("-----------Réponse-------------------\n%.*s", reponse->len, rep);
			// attention reponse->buf doit aussi pointer vers un espace memoire valide.
			if ((reponse->buf=malloc(strlen(rep))) != NULL ) {

				// on recopie la donnée dans la reponse
				strncpy(reponse->buf,rep,reponse->len);

				reponse->clientId=requete->clientId;

				// on envoi la reponse au client
				sendReponse(reponse);

				// reponse est recopiée par la bibliothèque, on peut tout de suite liberer la memoire
				free(reponse->buf);
			}
			free(reponse);

			//optionnel, ici on clot la connexion tout de suite (HTTP/1.0)
			requestShutdownSocket(reponse->clientId);
		}
		// on ne se sert plus de requete a partir de maintenant, on peut donc liberer...
		freeRequest(requete);
	}
	return (1);

}
