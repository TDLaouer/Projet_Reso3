#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "httpparser.h"
#include "api.h"
#include "request.h"
#include "semantic.h"

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



		/* Parseur */
		if (argc < 2 ) { printf("Usage: httpparser <file> [search]\nAttention <search> is case sensitive\n\tDisplay tree with CRLF removed!!"); exit(1); }
		// mmap from file the request to parse
		if ((fi=open(argv[1],O_RDWR)) == -1) {
									perror("open");
									return -1;
					}
					if (fstat(fi, &st) == -1)           /* To obtain file size */
									return -1;
					if ((addr=mmap(NULL,st.st_size,PROT_WRITE,MAP_PRIVATE, fi, 0)) == MAP_FAILED)
									return -1;
		// search a specific token
		if (argc == 3 ) {
			p=argv[2];
			printf("searching for %s\n",p);
			while (*p) {
				if (*p=='-') { *p='_'; }
				p++;
			}
			p=argv[2];
		}
		// call the parser with respect to api.h function declaration
		if (res=parseur(addr,st.st_size)) {
			_Token *r,*tok;

			// get the root of the tree this is no longer opaque since we know the internal type with httpparser.h
			//void *root;
			Lnode *root;
			root=getRootTree();
			if (p != NULL) {
				r=searchTree(root,p);
				tok=r;
				while (tok) {
					int l;
					char *s;
					// node is no longer opaque
					Lnode *node;
					node=(Lnode *)tok->node;
					printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
					tok=tok->next;
				}
				purgeElement(&r);
			}
			purgeTree(root);
		} else {
			printf("NOK\n");
		}
		munmap(addr,st.st_size);
		close(fi);
		/* fin section parseur */


// ceci decrit la methode experimentale de renvoi de la reponse Cf API
#ifdef EXPERIMENTAL
		writeDirectClient(requete->clientId,REPONSE,strlen(REPONSE));
		endWriteDirectClient(requete->clientId);
		requestShutdownSocket(requete->clientId);
#else
// ceci decrit la methode legacy de renvoi de la reponse  Cf API
		// avant d'envoyer une réponse on doit allouer de la mémoire pour la reponse
		if ((reponse=malloc(sizeof(message))) != NULL ) {

			reponse->len=strlen(REPONSE);

			// attention reponse->buf doit aussi pointer vers un espace memoire valide.
			if ((reponse->buf=malloc(strlen(REPONSE))) != NULL ) {
				// on recopie la donnée dans la reponse
				strncpy(reponse->buf,REPONSE,reponse->len);
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
#endif
	// on ne se sert plus de requete a partir de maintenant, on peut donc liberer...
	freeRequest(requete);
	}
	return (1);
}
