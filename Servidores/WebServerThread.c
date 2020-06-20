#include "WebServerThread.h"
#include "SystemFunctions.h"
#include "AttendRequest.h"
#include "lib/ancillary.h"

/*Funcion que se encarga de crear el socket que sera utilizado por el web server.*/
void createSocket()
{
	_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if(_socketServer == -1)
	{
		ErrorExit("Se ha presentado un error al intentar crear el socket de conexión.");
	}
	print("Socket principal creado con exito.");
}

/*Avisamos al sistema operativo que hemos creado un socket y queremos que una nuestro programa a el.*/
void bindSocket()
{
	_serverAdress.sin_family = AF_INET;
    _serverAdress.sin_addr.s_addr = htonl(INADDR_ANY);
    _serverAdress.sin_port = htons(_serverPort);
    if(bind(_socketServer, (struct sockaddr *) &_serverAdress, sizeof(_serverAdress)) == -1)
    {
    	ErrorExit("Se ha presentado un error al intentar asociar el socket de conexion.");
    }
    print("Socket principal asociado con exito.");
}

/*Funcion que se encarga de escuchar las conexiones entrantes al servidor.*/
void listeningConnections()
{
	if(listen(_socketServer, MAX_CONNECTIONS) == -1)
	{
		ErrorExit("Se ha producido un error al intentar escuchar conexiones.");
	}
	print("Escuchando nuevas conexiones entrantes.");
}

//libera procesos hijos
void closeChildProcess()
{
	for(int i = 0; i < _processMax; i++)
	{
		kill(_childProcess[i], SIGINT);
	}
	free(_childProcess);
}

/* Funcion que se encarga en detectar señales para avisarle al servidor que sucede */
void detectCloseSignal(int pType)
{
	struct sigaction closeSignal;
	if(pType == 1)
	{
		closeSignal.sa_handler = exitServerChild;
	}
	else
	{
		closeSignal.sa_handler = exitServerThread;
	}
	sigemptyset(&closeSignal.sa_mask);
	closeSignal.sa_flags = 0;
   	sigaction(SIGINT, &closeSignal, NULL);
   	sigaction(SIGTERM, &closeSignal, NULL);
}


void initializeWebServer(int argc, char *argv[])
{
	int option;
	while((option = getopt(argc,argv,"n:w:p:h")) != -1) // pregunta previamente que si desea incluir argumentos fijos al levantar el servidor
	{
		//se toman los argumentos para guardarlos en sus respectivas variables
		switch (option)
		{
			case 'n':
				_processMax = charArrayToInt(optarg);
				break;
			case 'w':
				strcpy(_rootPath, optarg);
				break;
			case 'p':
				_serverPort = charArrayToInt(optarg);
				break;
		}
	}
	//se inicia el servidor en modo prethread
	startThreadingWebServer();

}

/*Funcion encargada de colocar la memoria compartida de procesos activos a ceros*/
void fillToZero()
{
	for(int i = 0; i < _processMax; i++)
	{
		*(_activesProcess + i) = 0;
	}
}

/* Funcion que cierra los procesos hijos */
void exitServerChild()
{
	close(_socketPair[1]);
    exit(EXIT_SUCCESS); 
}

/* Funcion que se encarga en desconectar el server en modo PreThread */
void exitServerThread()
{
	print("Cerrando socket principal.");
	close(_socketServer);
	print("Cerrando threads que se encuentran abiertos.");
	_controlFlag = 0;	
    exit(EXIT_SUCCESS); 
}

/* Funcion que se encarga de administrar los procesos nuevos */
int activeProcess()
{
	int count = 0;
	for(int i = 0; i < _processMax; i++)
	{
		if(*(_activesProcess + i) == 1)
		{
			count++;
		}
	}
	return count;
}

/*Funcion que verifica si existe algun proceso disponible para ser asignado a una nueva conexion entrante*/
int verifyProcessAvailable()
{
	int value = -1;
	for(int i = 0; i < _processMax; i++)
	{
		if(*(_activesProcess + i) == 0)
		{
			value = i;
			break;
		}
	}
	return value;
}

/*Funcion encargada de crear los sockets de comunicacion interna*/
void createSocketPair()
{
	if(socketpair(PF_UNIX, SOCK_STREAM, 0, _socketPair)) 
	{
		ErrorExit("Se ha presentado un error al intentar crear los sockets de comunicacion interna.");
    } 
    else 
    {
		print("Sockets de comunicacion interna establecidos correctamente.");
    }
}


void *connectionHandlerThread() 
{
   int newSocket;
   //se mantiene el socket escuchando las nuevas conexiones
   while(_controlFlagThread) 
   {
      pthread_mutex_lock(&_newSocketMutex);
      while (_newSocket == -1)
      {
        pthread_cond_wait(&_threadsCondition, &_newSocketMutex);
      }
      newSocket = _newSocket;
      _newSocket = -1;
      pthread_mutex_unlock(&_newSocketMutex);
      if ( newSocket > 0)
      {
      	attendIncomingRequest(newSocket, _rootPath, _serverPort, _processMax, _currentThreads);
		shutdown(newSocket, 2);
		close(newSocket);
		print("Se ha cerrado una conexion ya atendida.");
         pthread_mutex_lock(&_threadNumberMutex);
         _currentThreads--;
         pthread_mutex_unlock(&_threadNumberMutex);
         pthread_cond_signal(&_acceptCondition);
      }

   }
   pthread_exit(NULL);
}

/* Funcion que crea un nuevo thread para ejecutarlo */
void createThreads(int pThreadNumber) 
{
   _threadNumber = pThreadNumber;
   for (int i = 0; i < pThreadNumber; i++)
   {
      pthread_t thread;
      int rc = pthread_create(&thread, NULL, connectionHandlerThread, NULL);
      if (rc)
      {
         ErrorExit("Error no controlado al crear un nuevo thread.");
      }
   }
}

/* Funcion que inicia un thread ya creado - es la funcion principal */
void startThreadingWebServer()
{
   	_currentThreads = 0;
   	_controlFlagThread = 1;

   	print("Iniciando el servidor en modo pre-thread...");
   	detectCloseSignal(1);
   	createSocket();
	bindSocket();
	createThreads(_processMax);
	listeningConnections();

   	while (_controlFlagThread) 
   	{
   		_incomingConnectionFileHandler = accept(_socketServer, NULL, NULL);
   		print("Nueva conexion entrante aceptada.");
   		if (_incomingConnectionFileHandler < 0) 
      	{
      		ErrorExit("Se ha presentado un error al intentar aceptar una nueva conexion entrante.");
      	} 
      	else 
      	{
         	pthread_mutex_lock(&_newSocketMutex);
         	_newSocket = _incomingConnectionFileHandler;
         	pthread_mutex_unlock(&_newSocketMutex);
         	pthread_cond_broadcast(&_threadsCondition);
         	pthread_mutex_lock(&_threadNumberMutex);
         	_currentThreads++;
         	if (_threadNumber == _currentThreads)
         	{ 
            	pthread_cond_wait(&_acceptCondition, &_threadNumberMutex);
         	}
         	pthread_mutex_unlock(&_threadNumberMutex);
      	}	
   }
   pthread_exit(NULL);
}

