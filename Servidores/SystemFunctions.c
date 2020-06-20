#include "SystemFunctions.h"

/** 
Funciones independientes que ayudan a la implementacion de los servidores web.
Se encuentran aparte para poder reducir el codigo fuente y modularizar.
**/

void ErrorExit(char const * pErrorMessage)
{
	fprintf(stderr, "Web Server: %s\n", pErrorMessage);
    exit(EXIT_FAILURE);
}

void print(char const * pMessage)
{
	fprintf(stdout, "Web Server: %s\n", pMessage);
}

void stringUpper(char *pReadBuffer) 
{
    while (*pReadBuffer != '\0') 
    {
		*pReadBuffer = toupper(*pReadBuffer);
		pReadBuffer++;
    }
}

void checkContentType(char *pResource, char **pContentType)
{
	char *auxPtrOne = strchr(pResource, '.');
	char *auxPtrTwo = calloc(4, sizeof(char));
	auxPtrOne++;
	while(*auxPtrOne != '\0')
	{
		strncat(auxPtrTwo, auxPtrOne, 1);
		auxPtrOne++;
	}
	stringUpper(auxPtrTwo);
	if(!strcmp(auxPtrTwo, "HTML"))
	{
		*pContentType = "text/html";
	}
	else if(!strcmp(auxPtrTwo, "CGI"))
	{
		*pContentType = "text/html";
	}
	else if(!strcmp(auxPtrTwo, "JPEG"))
	{
		*pContentType = "image/jpeg";
	}
	else if(!strcmp(auxPtrTwo, "PNG"))
	{
		*pContentType = "image/png";
	}
	else if(!strcmp(auxPtrTwo, "TXT"))
	{
		*pContentType = "text";
	}
	else if(!strcmp(auxPtrTwo, "JPG"))
	{
		*pContentType = "image/jpg";
	}
	else if(!strcmp(auxPtrTwo, "CSS"))
	{
		*pContentType = "text/css";
	}
	else if(!strcmp(auxPtrTwo, "JS"))
	{
		*pContentType = "application/javascript";
	}
	else if(!strcmp(auxPtrTwo, "XML"))
	{
		*pContentType = "application/xml";
	}
	else if(!strcmp(auxPtrTwo, "MP3"))
	{
		*pContentType = "audio/mpeg";
	}
	else if(!strcmp(auxPtrTwo, "MPEG"))
	{
		*pContentType = "video/mpeg";
	}
	else if(!strcmp(auxPtrTwo, "MPG"))
	{
		*pContentType = "video/mpeg";
	}
	else if(!strcmp(auxPtrTwo, "MP4"))
	{
		*pContentType = "video/mp4";
	}
	else if(!strcmp(auxPtrTwo, "MOV"))
	{
		*pContentType = "video/quicktime";
	}
	else
	{
		*pContentType = "application/octet-stream";
	}
	free(auxPtrTwo);
}

int charArrayToInt(char *pArray)
{
	return atoi(pArray);
}

void decodeURL(char *pReadBuffer) 
{
    char asciiNum[3] = {0};
    int c;
    int i = 0;
    
    while ( pReadBuffer[i] ) 
    {
		if ( pReadBuffer[i] == '+' )
		{
			pReadBuffer[i] = ' ';
		}
		else if ( pReadBuffer[i] == '%' ) 
		{
		    asciiNum[0] = pReadBuffer[i + 1];
		    asciiNum[1] = pReadBuffer[i + 2];
		    pReadBuffer[i] = strtol(asciiNum, NULL, 16);
		    c = i + 1;
		    do 
		    {
				pReadBuffer[c] = pReadBuffer[c + 2];
		    } while (pReadBuffer[2 + (c++)]);
		}
		i++;
    }
}

int writeLine (int pConnection, char *pReadBuffer, int pNumberCharacters)
{
	int sentBytes;
	int unsentBytes = pNumberCharacters;

	_result = 1;

    while (unsentBytes > 0) 
    {
    	sentBytes = send(pConnection, pReadBuffer, unsentBytes, 0);
		if (sentBytes <= 0 ) 
		{
		    if (errno == EINTR)
		    {
		    	sentBytes = 0;
		    }
		    else
		    {
		    	_result = -1;
		    	break;
		    }
		}
		unsentBytes -= sentBytes;
		pReadBuffer += sentBytes;
    }
    return _result;
}

int readLine(int pConnection, char **pReadBuffer, int pMaxRead, int pPosContent)
{
	char *readBufferAux = *pReadBuffer;
	char *readBufferAuxTwo = *pReadBuffer;
	int countRealloc = 2;
	int initialMaxRead = pMaxRead;
	_result = -1;
	char c;
    for (int i = 0; i < pMaxRead; i++) 
    {
    	int valueAux = recv(pConnection, &c, 1, 0);
		if (valueAux == 1 ) 
		{
		    *readBufferAuxTwo++ = c;
		    if (c == '\n' && pPosContent == 0)
		    {
		    	_result = 1;
		    	break;
		    }
		    if(i + 2 == pMaxRead)
		    {
		    	pMaxRead = countRealloc * initialMaxRead;
		    	char *auxPtr = realloc(readBufferAux, countRealloc * initialMaxRead);
		    	*pReadBuffer = auxPtr;
		    	readBufferAuxTwo = auxPtr;
		    	readBufferAux = auxPtr;
		    	auxPtr = NULL;
		    	readBufferAuxTwo = readBufferAuxTwo + i + 1;
		    	countRealloc++;
		    }
		    if(pPosContent != 0)
		    {
		    	if(i + 1 == pPosContent)
		    	{
		    		_result = 1;
		    		break;	
		    	}
		    }
		}
		else if (valueAux == 0 ) 
		{
			_result = 0;
			break;
		}
		else 
		{
			_result = -1;
			break;
		}
    }
    return _result;
}
