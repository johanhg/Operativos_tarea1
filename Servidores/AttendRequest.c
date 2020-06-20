#include "AttendRequest.h"
#include "ParseRequest.h"
#include "SystemFunctions.h"
#include "Resource.h"
#include "RespondRequest.h"
#include "CGIExecution.h"

char *_serverFilesPath = "/home/wal/Escritorio/Operativos/server/WebServerC-master2/Files";

//Por defecto tenemos el puerto 8080 y agregamos 80 antes de los puertos por defecto de cada protocolo
void attendIncomingRequest(int pConnection, char *pRootPath, int pPort, int pProcessMax, int pActiveProcess)
{
	if(pPort == 8080)
	{
		attendIncomingHttpRequest(pConnection, pRootPath);
	}
	else if(pPort == 8021)
	{
		attendIncomingFtpRequest(pConnection, pActiveProcess, pProcessMax);
	}
	else if(pPort == 8022)
	{
		attendIncomingSSHRequest(pConnection);
	}
	else if(pPort == 8023)
	{
		attendIncomingTelnetRequest(pConnection);
	}
	else if(pPort == 8025)
	{
		attendIncomingSMTPRequest(pConnection);
	}
	else if(pPort == 8053)
	{
		attendIncomingDNSRequest(pConnection);
	}
	else if(pPort == 8161)
	{
		attendIncomingSNMPRequest(pConnection);
	}
	else
	{
		attendIncomingHttpRequest(pConnection, pRootPath);
	}
}

void attendIncomingDNSRequest(int pConnection)
{
	char *welcomeDNS = "NOT IMPLEMENTED.\r\n";
	writeLine(pConnection, welcomeDNS, strlen(welcomeDNS));
    sleep(1);
}

void attendIncomingSSHRequest(int pConnection)
{
	char *welcomeSSH = "La autenticación del servidor ha sido exitosa!.\r\n";
	char *rsa = "La huella dactilar es 97:4f:66:f5:96:ba:6d:b2:ef:65:35:45:18:0d:cc:29 \r\n";
	char *continueServer = "Estás segure de mantener la conexión (yes/no)?\r\n";
	writeLine(pConnection, welcomeSSH, strlen(welcomeSSH));
	writeLine(pConnection, rsa, strlen(rsa));
	writeLine(pConnection, continueServer, strlen(continueServer));
    sleep(1);
}

/*Funcion que se encarga de atender una nueva peticion*/
void attendIncomingHttpRequest(int pConnection, char *pServerRootPath)
{
	struct RequestInfo _requestInfo;
	char *_contentType = "text/html";

	initRequestInfo(&_requestInfo);
	int auxValue = parseRequestInfo(pConnection, &_requestInfo);
	if(auxValue == -1)
	{
		_requestInfo._status = 500;
		auxValue = respondRequest(pConnection, &_requestInfo, _contentType);
		if(auxValue == -1)
		{
			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
		}
	}
	else if(auxValue == 0)
	{
		print("Al parecer el cliente se ha desconectado.");
	}
	else
	{
		attendOutputRequest(pConnection, &_requestInfo, pServerRootPath, _contentType);
	}
    sleep(1);
}

void doNothing(int pConnection)
{
	char c;
	int valueAux;
	do
	{
		valueAux = recv(pConnection, &c, 1, 0);
	}while(valueAux != 0);
}

void attendIncomingFtpRequest(int pConnection, int pActiveProcess, int pProcessMax)
{
	int activeProcessInt = pActiveProcess;
	char activeProcessStr[21];
	char maxProcessStr[21];
	sprintf(activeProcessStr, "%d", activeProcessInt);
	sprintf(maxProcessStr, "%d", pProcessMax);
	char ftpResponse[85] = "220 FTP server, ";
	strcat(ftpResponse, activeProcessStr);
	strcat(ftpResponse, " Clientes activos de ");
	strcat(ftpResponse, maxProcessStr);
	strcat(ftpResponse, " Simulación de clientes permitidos.\r\n");
	writeLine(pConnection, ftpResponse, strlen(ftpResponse));
	sleep(1);
}

void attendIncomingSNMPRequest(int pConnection)
{
	char *versionSNMP = "Version: 1\r\n";
	char *community = "Community: public \r\n";
	char *pdu = "PDU type: GET\r\n";
	char *request = "Request Id: 0\r\n";
	char *error = "Error Status: NO ERROR\r\n";
	char *errorIndex = "Error Index: 0\r\n";
	writeLine(pConnection, versionSNMP, strlen(versionSNMP));
	writeLine(pConnection, community, strlen(community));
	writeLine(pConnection, pdu, strlen(pdu));
	writeLine(pConnection, request, strlen(request));
	writeLine(pConnection, error, strlen(error));
	writeLine(pConnection, errorIndex, strlen(errorIndex));
	sleep(1);
}

void attendIncomingTelnetRequest(int pConnection)
{
	char *welcomeTelnet = "Bienvenido al servidor...\r\n";
	char *loginMsg = "Login: \r\n";
	char *passwordMsg = "Password: \r\n";
	writeLine(pConnection, welcomeTelnet, strlen(welcomeTelnet));
	writeLine(pConnection, loginMsg, strlen(loginMsg));
	writeLine(pConnection, passwordMsg, strlen(passwordMsg));
	sleep(1);
}

void attendIncomingSMTPRequest(int pConnection)
{
	char *welcomeSMTP = "220 Server SMTP\r\n";
	writeLine(pConnection, welcomeSMTP, strlen(welcomeSMTP));
	sleep(1);
}


void attendOutputGetRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType)
{
	if(!strcmp(pRequestInfo->_resource, "/"))
	{
		pRequestInfo->_resource = "/index.html";
		pServerRootPath = _serverFilesPath;
	}
	int resource = checkResource(pRequestInfo, pServerRootPath);
	if ( resource < 0) 
	{
	    if (errno == EACCES)
	    {
	    	pRequestInfo->_status = 401;
	    }
	    else
	    {
	    	pRequestInfo->_resource = "/404.html";
	    	resource = checkResource(pRequestInfo, _serverFilesPath);
	    	pRequestInfo->_status = 404;
	    }
	}
	checkContentType(pRequestInfo->_resource, &pContentType);
	int auxValue = respondRequest(pConnection, pRequestInfo, pContentType);
	if(auxValue == -1)
	{
		print("Ocurrio un error grave en el servidor el cual no fue controlado.");
	}
	else
	{
		if (pRequestInfo->_status == 200 || pRequestInfo->_status == 404) 
		{
			if (sendResource(pConnection, resource) < 0)
			{
				print("No se pudo enviar el archivo al destinatario.");
			}
			close(resource);
		}
	}
}

void attendOutputRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType)
{
	if(pRequestInfo->_method == UNSUPPORTED)
	{
		pRequestInfo->_status = 501;
		int auxValue = respondRequest(pConnection, pRequestInfo, pContentType);
		if(auxValue == -1)
		{
			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
		}
	}
	else if(pRequestInfo->_method == GET)
	{
		attendOutputGetRequest(pConnection, pRequestInfo, pServerRootPath, pContentType);
	}
}
