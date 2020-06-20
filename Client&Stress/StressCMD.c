#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int var = 0;
int x = 0;

int main(int argc, char **argv) {
   char execute[50]; //Sentencia que la terminal debe ejecutar
   char numThreads[10]; 
   //Se genera la estructura de la sentencia
   strcpy(execute,"./");
   strcat(execute,argv[2]);
   strcat(execute," ");
   strcat(execute,argv[3]);
   strcat(execute," ");
   strcat(execute,argv[4]);
   strcpy(numThreads,"");
   strcat(numThreads,argv[1]);
   int num = atoi(numThreads);
   
   pid_t pidC;
	
	if (argc != 5) {
		printf("USAGE: ./StressCMD <number of threads> httpclient <hostname> <port>\n");
		return 1;
        }else{
			pidC = fork(); //Instanciamos un proceso hijo
                        
			//Creamos hilos dinamicamente con el argumento
			for(int i=1;i<(num);i++){
				if(pidC > 0){
				pidC = fork();
				}
			}

			if(pidC == 0){
				system(execute);
			}
		}

	if(pidC > 0 ){
	     sleep(3);
	     printf("Las peticiones han sido ejecutadas con éxito\n");				

}

   return 0;
}



