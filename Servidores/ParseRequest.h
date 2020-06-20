#ifndef _PARSEREQUEST_H
#define _PARSEREQUEST_H

#define MAX_REQUEST_LINE 1024

#include <sys/time.h>
#include <string.h>
#include <ctype.h>

enum _requestMethods { GET, POST, PUT, DELETE, UNSUPPORTED};
int _result;
int _actualLine;
int _controlFlag;
int _detectContentPost;
char *_contentLenghtTemp;

struct RequestInfo
{
    enum _requestMethods _method;
    char *_resource;
    char *_content;
    int _contentLength;
    int _status;
};

int extractMethod(char **pReadBuffer, struct RequestInfo *pRequestInfo); /*Funcion que se encarga de extraer el metodo solicitado por el request*/
int parseRequestInfo(int pConnection, struct RequestInfo *pRequestInfo); /*Funcion encargada de parsear los datos que vienen en las cabezaras de las peticiones*/
void parseLineHeader(char *pReadBuffer, struct RequestInfo *pRequestInfo); /*Funcion encargada de parsea una linea dada de las cabezeras HTTP*/
void initRequestInfo(struct RequestInfo *pRequestInfo); /*Funcion que pone valores por default en la estructura*/
void extractRecourse(char *pReadBuffer, struct RequestInfo *pRequestInfo); /*Funcion encargada de extraer el recurso solicitado por el request*/

#endif
