#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "semantic.h"
#define MAX 150

#define REPONSE "HTTP/1.0 200 OK\r\n\r\n"


int main(int argc, char *argv[])
{
	message *requete;
	message *reponse;
	while ( 1 ) {
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest.
		if ((requete=getRequest(8080)) == NULL ) return -1;

		// Affichage de debug
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId);
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);

		int r = parseur(requete->buf, requete->len);
		char *rep = (char*)malloc(MAX*sizeof(char));
		rep=returnInChar(Verification());

		if ((reponse=malloc(sizeof(message))) != NULL ) {

			reponse->len=strlen(rep);

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
