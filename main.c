#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "semantic.h"

#define SUPERMAX 100000

#define REPONSE "HTTP/1.0 200 OK\r\n\r\n"
static long body_size;


char* server_fileToSend(char* file,int len) {
	char* sendbody;
	char* filename=(char*)calloc(SUPERMAX,sizeof(char));

	if (file[0]=='/'){
		strncpy(filename,file+1,len-1);
	}
	else{
		strncpy(filename,file,len);
	}
	printf(" filetruc %s\n",filename);
	FILE* f = fopen(filename, "rb");
	if(f == NULL) {
		f = fopen("www/notfound.html", "rb");
	}

	fseek(f, 0, SEEK_END);
	body_size = ftell(f);
	printf("%d\n", body_size);
	fseek(f, 0, SEEK_SET);

	if((sendbody = malloc(body_size + 1)) == NULL)
	exit(1);

	fread(sendbody, body_size, 1, f);
	fclose(f);
	strcat(sendbody, "\0");
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

		if (strcmp(rep, "HTTP/1.1 200 OK\r\nConnection: keep-alive\0") == 0 || strcmp(rep, "HTTP/1.1 200 OK\r\nConnection: close\0") == 0)
		{
			_Token * tmp;
			tmp = searchTree(getRootTree(),"method");
			Lnode * r = (Lnode*)tmp->node;
			int* lenMethod = 0;
			char* method = getElementValue(tmp->node ,lenMethod);
			printf("oui ? %s\n", method);
			if (strncmp(method, "GET",3) == 0)
			{
				_Token * tmp2;
				tmp2 = searchTree(getRootTree(), "request_target");
				Lnode* r2 = (Lnode*)tmp2->node;
				target = r2->value;
				printf("%s\n", target);
				strcpy(sendbody, server_fileToSend(target,r2->len));

				strcat(rep, sendbody);

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
