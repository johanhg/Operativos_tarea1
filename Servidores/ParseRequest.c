#include "ParseRequest.h"
#include "SystemFunctions.h"

/*Funcion que pone valores por default en la estructura*/
void initRequestInfo(struct RequestInfo *pRequestInfo)
{
	pRequestInfo->_method = UNSUPPORTED;
	pRequestInfo->_resource = NULL;
	pRequestInfo->_contentLength = 0;
	pRequestInfo->_content = NULL;
	pRequestInfo->_status = 200;
}

/*Funcion que se encarga de extraer el metodo solicitado por el request*/
int extractMethod(char **pReadBuffer, struct RequestInfo *pRequestInfo)
{
	if (!strncmp(*pReadBuffer, "GET", 3)) 
	{
	    pRequestInfo->_method = GET;
	    *pReadBuffer = *pReadBuffer + 4;
	    return 0;
	}
/**
	else if (!strncmp(*pReadBuffer, "POST", 4)) 
	{
    	pRequestInfo->_method = POST;
    	*pReadBuffer = *pReadBuffer + 5;
    	return 0;
	}
	else if (!strncmp(*pReadBuffer, "PUT", 3)) 
	{
	    pRequestInfo->_method = PUT;
	    *pReadBuffer = *pReadBuffer + 4;
	    return 0;
	}
	else if (!strncmp(*pReadBuffer, "DELETE", 6)) 
	{
	    pRequestInfo->_method = DELETE;
	    *pReadBuffer = *pReadBuffer + 7;
	    return 0;
	}
**/
	else
	{
		pRequestInfo->_method = UNSUPPORTED;
	    pRequestInfo->_status = 501;
    	_controlFlag = 0;
    	return 1;
	}
}

/*Funcion encargada de extraer el recurso solicitado por el request*/
void extractRecourse(char *pReadBuffer, struct RequestInfo *pRequestInfo)
{
	char *auxPtr = strchr(pReadBuffer, ' ');
	int len = auxPtr - pReadBuffer;
	pRequestInfo->_resource = calloc(len, sizeof(char));
	strncpy(pRequestInfo->_resource, pReadBuffer, len);
}

/*Funcion encargada de parsea una linea dada de las cabezeras HTTP*/
void parseLineHeader(char *pReadBuffer, struct RequestInfo *pRequestInfo)
{

}

int parseRequestInfo(int pConnection, struct RequestInfo *pRequestInfo)
{
	char *readBuffer;
    fd_set fileHandlers;
    struct timeval timeWait;

    timeWait.tv_sec  = 5;
    timeWait.tv_usec = 0;

    _actualLine = 0;
    _contentLenghtTemp = 0;
    _detectContentPost = 0;
    _controlFlag = 1;
    _result = 1;

    while(_controlFlag)
    {
		FD_ZERO(&fileHandlers);
		FD_SET (pConnection, &fileHandlers);

		if (select(pConnection + 1, &fileHandlers, NULL, NULL, &timeWait) == -1) 
		{
		    _result = -1;
		    break;
		}
		else if (FD_ISSET(pConnection, &fileHandlers)) 
		{
			readBuffer = calloc(MAX_REQUEST_LINE, sizeof(char));
		    int valueAux = readLine(pConnection, &readBuffer, MAX_REQUEST_LINE, pRequestInfo->_contentLength);
		    if(valueAux == -1)
		    {
		    	_result = -1;
		    	break;
		    }
		    else if(valueAux == 0)
		    {
		    	_result = 0;
		    	break;
		    }
		    else
		    {
		    	parseLineHeader(readBuffer, pRequestInfo);
		    }
		    free(readBuffer);
		}
    }
    return _result;
}
