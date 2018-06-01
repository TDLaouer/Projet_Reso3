#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "request.h"
#include "table.h"
/*
bool verifPchar(char* answer, int k) {
	if ( answer[k] == '%') {
		if ( answer[k+1]!='\0') {
			if ( answer[k+1]!='\0'){
				if ( !verifHexa(answer[k+1])  || !verifHexa(answer[k+2] )) {
					return false;
				}
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	}
	return ( verifUnreserved(answer[k]) ||  verifSubdelims(answer[k]) || answer[k]=='@' || answer[k]==':');
}
*/


int verifSemMethod(char* theresponse, int taille) {
	int i=0;
	char Target[150];
	char Body[500];
	char method[20] = "";
	strcpy(method, Print_Table("method"));
	if (strcmp(method,"GET") || strcmp(method,"HEAD")) {
		strcpy(Target,Print_Table("target"));
		if(Target[i]=='/'){
			i++;
			while ( Target[i]!='\0' && Target[i]!='?') {
				if(verifPchar(Target, i)|| Target[i] =='/') {
					i++;
				}else{
					strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n");
					return 0 ;
				}
			}
			if(Target[i]=='?') {
				i++;
				while ( Target[i]!='\0') {
					if(verifPchar(Target, i) || Target[i]!='?' || Target[i]!='/') {
						i++;
					}else{
						strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n");
						return 0;
					}
				}
			}
		}
			return 1;
	}else if (strcmp(method,"POST")) {
		/*4 possibilit� d'usage:	- "providing a block of data"
							- "posting a message"
							- cr�� une nouvelle resource
							- " appenting data"
		a tait� en partie 4 du projet*/

	}else if (strcmp(method,"PUT")) {
	/* replace l'etat de la target par l'etat defini (en ignorant les headers invalides)
	?????????????????????????????????????????????????????????????

	doit renvoyer une erreur bed request HTTP/1.1 400 lorsque elle contient le champ entete content-range */


	}else if (strcmp(method,"DELETE")) {
		return 1 ;
	}else if (strcmp(method,"CONNECT")) {
		return 1;
	}else if (strcmp(method,"OPTIONS")) {
		return 1;
	}/*else if (strcmp(method,"TRACE")) {
		strcopy(Body,Print_Table("body"));
		if (Body[0] =! '\0') {
			for(i==0; i<9; i++) {
				if (getCompteur(requ[i])!=0)) {
					strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n\r\n")
					return 0;
				}
			}
			return 1;
		}
		strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n\r\n")
		return 0;
	}*/
	strcpy(theresponse,"HTTP/1.1 405 Method not allowed\r\n");
	return 0;
}


int verifSemCookie(char* theresponse, int taille) {
	if (getCompteur("Cookie")>=2){
		strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n");
		return 0;
	}
	return 1;
}


int verifTrasfertEncoding(char* theresponse, int taille) {
	if (getCompteur("Transfer-Encoding")>=2){
		strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n");
		return 0;
	}
	return 1;
}



int verifSemAccept(char* theresponse, int taille) {
	if (getCompteur("Accept")>=2){
		strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n");
		return 0;
	}
	return 1;
}



int verifSemAcceptEncoding(char* theresponse, int taille) {
	if (getCompteur("Accept-Encoding")>=2){
		strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n");
		return 0;
	}
	return 1;
}



int verifSemReferer(char* theresponse, int taille) {
	if (getCompteur("Referer")>=2){
		strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n");
		return 0;
	}
	return 1;
}

int verifSemUserAgent(char* theresponse, int taille) {
	if (getCompteur("User-Agent")>=2){
		strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n");
		return 0;
	}
	return 1;
}



int verifSemConnection(char* theresponse, int taille) {
	char connection[12];
	char version[3];
	strcpy(connection, Print_Table("Connection"));
	strcpy(version, Print_Table("version"));
	/* On aura forc�ment pass� la verifVersion a ce moment */
	if (strcmp(connection, "close") != 0 && strcmp(connection, "keep-alive") != 0 && strlen(connection) != 0){
		strcpy(theresponse, "HTTP/1.1 400 Bad Request\r\n");
		return 0;
	}
	else if (strcmp(connection, "close") == 0) {

		return 1;
	}

	else if (version[2] == '1' && version[0] == '1') {
		return 2;
	}

	else if (strcmp(connection, "keep-alive") == 0) {
		return 2;
	}

	else{
		return 1;
	}
}



int verifSemHost(char* theresponse, int taille) {   /* TEST SI LE CHAMP HOST EST VIDE OU PAS, IL DEVRAIT PAS ETRE VIDE */
	char host[50];
	strcpy(host, Print_Table("Host"));

	if (getCompteur("User-Agent")>=2){
		strcpy(theresponse, "HTTP/1.1 400 Bad request\r\n");
		return 0;
	}

	if (strlen(host) == 0) {
		strcpy(theresponse, "HTTP/1.1 400 Bad Request\r\n");
		return 0;
	}
	return 1;
}



int verifSemVersion(char* theresponse, int taille) {
	char version[3];
	strcpy(version, Print_Table("version"));
	if (version[0] != '1' || (version[2] != '0' && version[2] != '1')) {
		strcpy(theresponse, "HTTP/1.1 505 HTTP Version Not Supported\r\n");
		return 2;
	}

	else if (version[2] == '1') {   /* SI ON EST EN VERSION 1.1 IL DOIT Y AVOIR UN CHAMP HOST, QUI NE PEUT PAS �TRE VIDE CAR ORIGIN FORM */
		if (verifSemHost(theresponse, taille) == 0) {
			strcpy(theresponse, "HTTP/1.1 400 Bad Request\r\n");
			return 0;
		}
	}
	return 1;
}


int verifSemTransferEncoding(char* theresponse, int taille) {

	/* Si y a un Transfer Encoding il ne doit pas y avoir de content length  (dans la r�ponse pour les deux)*/

	return 1;


}


int verifSemContentLength(char* theresponse, int taille) {
	char contentlength[10];
	strcpy(contentlength, Print_Table("Content-Length"));
	if (getCompteur("Transfer-Encoding") != 0) {
		strcpy(theresponse, "HTTP/1.1 400 Bad Request\r\n");
		return 0;
	}
	return 1;
}





void construireReponse(char* theresponse, int taille, int flag) {
	int indice = 0;
	char version[3];
	strcpy(version, Print_Table("version"));
	while(theresponse[9] == '2' && indice < 9) {   /* tant que le premier carac est le 2 de 200 OK et qu'on a pas parcouru la liste des headers */
		if (indice == 0) {
			if(getCompteur("Cookie") != 0) {
				verifSemCookie(theresponse, taille);
			}
		}
		else if (indice == 1) {    /* verif Connection doit happen m�me si le header n'est pas pr�cis� */
			flag = verifSemConnection(theresponse, taille);
			if (flag == 1) {
				taille = taille + 20;
				/*theresponse = realloc(theresponse, taille);
				if (theresponse == NULL) {
					exit(1);
				}*/
				strcat(theresponse, "Connection: close\r\n\0");
			}
			else if(flag == 2) {
				taille = taille + 25;
				/*theresponse = realloc(theresponse, taille);
				if (theresponse == NULL) {
					exit(1);
				}*/
				strcat(theresponse, "Connection: keep-alive\r\n\0");
			}
		}
		else if (indice == 2) {
			if(getCompteur("Transfer-Encoding") != 0) {
				verifSemTransferEncoding(theresponse, taille);
			}
		}
		else if (indice == 3) {
			if(getCompteur("Host") != 0) {
				verifSemHost(theresponse, taille);
			}
		}
		else if (indice == 4) {
			if(getCompteur("Referer") != 0) {
				verifSemReferer(theresponse, taille);
			}
		}
		else if (indice == 5) {
			if(getCompteur("User-Agent") != 0) {
				verifSemUserAgent(theresponse, taille);
			}
		}
		else if (indice == 6) {
			if(getCompteur("Accept") != 0) {
				verifSemAccept(theresponse, taille);
			}
		}
		else if (indice == 7) {
			if(getCompteur("Accept-Encoding") != 0) {
				verifSemAcceptEncoding(theresponse, taille);
			}
		}
		else if (indice == 8) {
			if(getCompteur("Content-Length") != 0) {
				verifSemContentLength(theresponse, taille);
			}
		}
		indice++;

	}
	return;

}
