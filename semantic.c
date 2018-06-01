#include "semantic.h"


char* Semantique(Lnode * root){
  int taille = 80085;
  char* answer = calloc(taille, sizeof(char));
  int result;


  result = VerifHTTPVersion(list, answer);
  if (result == -1)
    return answer;
  else if (result == 0)
  {
    strcpy(answer, "HTTP/1.1 400 Bad Request\r\n");
    return answer;
  }
  else if (result == 2)
  {
    result = verifHost(list, answer);
  }
}



/* permettra de compter les token de réponse en cas de preuve d'unicité*/
int CountToken (_Token * list){
  int count = 0;
  while (list != NULL)
  {
    count++;
    list = list->node->link;
  }
  return count;
}


/* pas besoin de prouver l'unicité */
/* Les fonctions Verif retournent -1 si incorrecte semantiquement, 0 si le liste
 de noeud est vide et 1 si pas de problemes */
int VerifMethod (char* answer){
  char* method, * Target;
  _Token* t = searchTree(list,"request-target" );
  int r = CountToken(list),tc = CountToken(t),  i=0;
  int resTarget;
  if (r!=1 || tc !=1){ /* plusieurs méthodes ou pas de méthode, plusieurs ou pas de request-target*/
    strcpy(answer, "HTTP/1.1 400 Bad request\r\n");
    return -1;
  }

  strncpy(method, list->node->value, list->node->len);

  if (strcmp(method,"GET") || strcmp(method,"HEAD")) {

    resTarget = VerifTarget(answer);
    if(resTraget == 0)
      return 0;
    return 1;

	}
  else if (strcmp(method,"POST"))
  {
		/*4 possibilit� d'usage:	- "providing a block of data"
							- "posting a message"
							- cr�� une nouvelle resource
							- " appenting data"
		a tait� en partie 4 du projet*/
    return 1;
	}
  else
  {
    strcpy(answer,"HTTP/1.1 405 Method not allowed\r\n");
  	return 0;
  }
  return 1;
}


/* pas besoin de prouver l'unicité */
int VerifTarget (char* answer){
  char *target;
  _Token* t = searchTree(getRootTree(),"request-target" );
  strncpy(Target, t->node->value, t->node->len);
  /*message d'erreur???????????????????*/
  if(Target[i]=='/'){
    i++;
    while ( Target[i]!='\0' && Target[i]!='?') {
      if(verifPchar(Target, i)|| Target[i] =='/') {
        i++;
      }
      else{
        strcpy(answer, "HTTP/1.1 400 Bad request\r\n");
        return 0 ;
      }
    }
    if(Target[i]=='?') {
      i++;
      while ( Target[i]!='\0') {
        if(verifPchar(Target, i) || Target[i]!='?' || Target[i]!='/') {
          i++;
        }
        else{
          strcpy(answer, "HTTP/1.1 400 Bad request\r\n");
          return 0;
        }
      }
    }
  }
  return 1;
}


/* pas besoin de prouver l'unicité */
int VerifHTTPVersion (char* answer){
  /*vérification de la méthode*/
  _Token* list = searchTree(getRootTree(), "version");
  if (list == NULL) {
    return 0;
  }
	else if (list->value != "1.0" || list->value != "1.1")
  {
		strcpy(answer, "HTTP/1.1 505 HTTP Version Not Supported\r\n");
		return -1;
  }
	elseif (list->value == "1.0")
  {
    return 1;
  }
	else if (list->value == "1.1")
  {
    return 2;
  }

 /*HTTP1.1 alors vérification champ HOST obligatoire*/
 /*donc nécésite CountToken après un searchTree de HOST*/

 /*si différent de 1.0 ou 1.1 alors "505 HTTP Version not supported"*/
 return 1;
}


/* nécésite unicité ??????????????????????????????????????????????????? */
int VerifHost (char* answer){
  int count = CountToken(list);
  if (count == 0)
    return 0;

  if (count != 1 || count != 0)
  {
    strcpy(answer, "HTTP/1.1 400 Bad request\r\n");
  }
  if (count == 0)
    return 0;
  /*si faux "400 bad request"????????????*/
  return 1;
}


/* nécésite unicité */
int verifAccept (char* answer){
  /*test unicité*/
  /*non unique "400 bad request"????????????*/

  /*vérification reste si présent*/
  return 1;
}


/* nécésite unicité */
int verifAcceptEncoding (char* answer){
  /*test unicité*/
  /*non unique "400 bad request"????????????*/

  /*vérification reste si présent*/
  return 1;
}


/* nécésite unicité */
int verifUserAgent (char* answer){
  /*test unicité*/
  /*non unique "400 bad request"????????????*/

  /*vérification reste si présent*/
  return 1;
}


/* nécésite unicité */
int verifTransferEncoding (char* answer){
  /*test unicité*/
  /*non unique "400 bad request"????????????*/

  /*vérification reste si présent*/
  return 1;
}


/* nécésite unicité */
int VerifCookie (char* answer, _Token * list){
  /*test unicité*/
  /*non unique "400 bad request"????????????*/

  /*vérification reste si présent*/
  return 1;
}


/* nécésite unicité */
int VerifReferer (char* answer, _Token * list){
  /*test unicité*/
  /*non unique "400 bad request"????????????*/

  /*vérification reste si présent*/
  return 1;
}


/* nécésite unicité ??????????????????????????????????????????????????? */
int VerifContentLength (char* answer, _Token * list){

  /*si faux "400 bad request"????????????*/
  return 1;
}


/* nécésite unicité ??????????????????????????????????????????????????? */
int VerifConnection (char* answer, _Token * list){

  /*si faux "400 bad request"????????????*/
  return 1;
}
