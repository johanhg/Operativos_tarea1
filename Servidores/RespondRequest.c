#include "RespondRequest.h"
#include "SystemFunctions.h"

//Verifica que el servidor se encuentre activo y lo prepara para recibir solicitudes
int respondRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pContentType)
{
	char bufferAux[100];
	char bufferAuxContent[100];
    sprintf(bufferAux, "HTTP/1.1 %d OK\r\n", pRequestInfo->_status);
    sprintf(bufferAuxContent, "Content-Type: %s\r\n", pContentType);
	char *headersArray[5] = {bufferAux, "WebServer V0.1\r\n", bufferAuxContent, "Connection: keep-alive\r\n", "\r\n"};
	int auxValue = 1;
	int count = 0;
	while(auxValue != -1 && count < 5)
	{
		auxValue = writeLine(pConnection, headersArray[count], strlen(headersArray[count]));
		count++;
	}
    return auxValue;
}
