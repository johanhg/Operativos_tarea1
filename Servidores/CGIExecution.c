#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#include "SystemFunctions.h"
#include "CGIExecution.h"

//Funcion que ejecuta una peticion CGI.
char* executeCGI(char* pPath, char **pArgs, char **pArgenv)
{

	int pipefd[2];
	pipe(pipefd);

	if (fork() == 0)
	{
	    while ((dup2(pipefd[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
	    close(pipefd[1]);
		close(pipefd[0]);
	    execve(pPath, (char * const*)pArgs, (char * const*)	pArgenv);
	    exit(1);
	}
    wait(0);
    char buffer[4096];
	while (1) 
	{
		ssize_t count = read(pipefd[0], buffer, sizeof(buffer));
		if (count == -1) 
		{
			if (errno == EINTR) 
			{
				continue;
			} 
			else 
			{
				perror("Error leyendo la respuesta de CGI.");
				exit(1);
			}
		} 
		else if (count == 0) 
		{
			break;
		} 
		else 
		{
			break;
		}
	}
	close(pipefd[0]);
	char* mssg = malloc(sizeof(char)*4096);
	strcpy(mssg, buffer);
	return mssg;
}

//Funcion GET del CGI
char* executeGetCGI(char* pPath, char* pQueryString)
{
	char* pathName = malloc(sizeof(char)*26);
	strcpy(pathName, "./");
	strcat(pathName, pPath);

	char* strQuery = malloc(sizeof(char)*200);
	strcpy(strQuery, "QUERY_STRING='");
	strcat(strQuery, pQueryString);
	strcat(strQuery, "'");

	char* args [] = {pathName, NULL};
	char* argenv [] = {strQuery, NULL};
	return executeCGI(pPath, args, argenv);
}


