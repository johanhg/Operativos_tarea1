#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>

/*Defines para el servidor tipo fork y thread*/

#define MAX_CONNECTIONS 1024 /*Definimos un limite de 1024 conexiones maximas encoladas.*/
#define MAX_PATH 1024
#define FILE_KEY "/bin/ls" /*Definimos la ruta que usaremos como acceso para la memoria compartida*/
#define KEY_ACTIVE_PROCESS 150 /*Definimos el entero que usaremos como llave para la memoria compartida de los procesos activos*/
#define KEY_INCOMING_CONNECTION 151 /*Definimos el entero que usaremos como llave para la memoria compartida de la conexion entrante*/


char _rootPath[MAX_PATH];
int *_childProcess;
int _serverPort;
int _socketServer; /*Definimos el socket en el cual correra el servidor.*/
int _processNumber; /*Definimos una variable para guardar el numero de proceso que sera igual a la posicion en el arreglo de procesos activos*/
int _processMax; /*Definimos una variable que guardara la cantidad maxima de procesos creados definidos por el usuario*/
int _incomingConnectionFileHandler; /*Definimos una variable que tendra el file handler de la conexion entrante.*/
int _idZoneActiveProcess;
int _idZoneIncomingConnection;
int _socketPair[2]; /*Definimos dos sockets los cuales serviran como comunicacion interna entre los procesos*/
pid_t _pidFork; /*Pid del nuevo procesado que sera creado por cada fork.*/
struct sockaddr_in _serverAdress; /*Definimos la estructura que va a contener todos los datos del servidor.*/
int *_activesProcess; /*Definimos un arreglo de memoria dinamica el cual se encargara de contener los procesos activos y no activos para atender solicitudes, activo = 1, no activo = 0*/
int *_incomingConnection; /*Definimos una puntero donde se guardara la conexion entrante.*/

/*Variables para el servidor tipo thread*/

int _threadNumber;
int _currentThreads;
int _newSocket;
int _controlFlagThread;
pthread_cond_t _threadsCondition;
pthread_cond_t _acceptCondition;
pthread_mutex_t _newSocketMutex;
pthread_mutex_t _threadNumberMutex;

/*Metodos para el servidor tipo fork*/

void createSocket(); /*Funcion que se encarga de crear el socket que sera utilizado por el web server.*/
void bindSocket(); /*Avisamos al sistema operativo que hemos creado un socket y queremos que una nuestro programa a el.*/
void listeningConnections(); /*Funcion que se encarga de escuchar las conexiones entrantes al servidor.*/
void initializeWebServer(int argc, char *argv[]);
void detectCloseSignal(int pType);
void exitServerThread();
void exitServerChild();
int activeProcess();
void closeChildProcess();
void acceptIncomingConnections(); /*Funcion encargada de aceptar conexiones entrantes y crear un nuevo proceso para cada una de ellas.*/
void startForkWebServer(); /*Funcion que se encarga de precrear los procesos disponibles para aceptar conexiones*/
void createSocketPair(); /*Funcion encargada de crear los sockets de comunicacion interna*/
void fillToZero(); /*Funcion encargada de colocar la memoria compartida de procesos activos a ceros*/
int verifyProcessAvailable(); /*Funcion que verifica si existe algun proceso disponible para ser asignado a una nueva conexion entrante*/

/*Metodos para el servidor tipo thread*/

void *connectionHandlerThread();
void createThreads(int pThreadNumber);
void startThreadingWebServer();

#endif
