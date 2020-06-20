# Operativos_tarea1
Crear un WebServer el cual utiliza enteramente el protocolo  HTTP/1.1 para su comunicación. Este WebServer podría ejecutarse en modo pre-threaded o pre-forked. Además crear un cliente web que busque vencer los webservers dejándolos sin posibilidad de atender  a otro cliente.

# Ejecución del programa

Para la ejecuci´on del programa se necesita el sistema operativo GNU/Linux.
Es necesario descargar el c´odigo fuente del proyecto, este se puede encontrar en el
repositorio de GitHub ubicado en https://github.com/johanhg/Operativos tarea1
Tras descargar el c´odigo fuente, hay que proceder a extraer el proyecto en alguna
carpeta de nuestro sistema operativo.
Seguidamente utilizamos la terminal y los comandos correspondientes para movernos hasta la direcci´on donde se encuentra nuestro proyecto. Este proyecto consta
de dos carpetas principales, la que contiene los servidores y la que contiene los
clientes.
Luego nos ubicaremos en la carpeta que contiene los servidores, vamos a proceder a compilar el proyecto utilizando el makefile, para ello debemos abrir la
terminal en con la direcci´on donde se encuentra el makefile y escribir make en
la terminal y teclear enter, esto nos va a compilar todos los archivos .c y nos va
a crear los ejecutables. Seguidamente podemos levantar nuestro propio servidor
de dos formas:
En modo pre-thread utilizaremos la sintaxis:
./WebServerThread -n [cantidad-hilos] -w [path] -p [port]
En modo pre-forked la sintaxis ser´a:
./WebServerFork -n [cantidad-hilos] -w [path] -p [port]
Es importante destacar que para nuestro proyecto el [path] indica la carpeta
del proyecto ”Files”, es decir, se debe escribir la ruta del proyecto que apunta
a esa carpeta en espec´ıfico.
Utilizando los comandos anteriores podemos levantar nuestros propios servidores web lo que nos permitiria levantar los clientes que se conecten al servidor
y puedan hacer alguna solicitud, para esto debemos abrir una terminal nueva y
ubicarnos en la carpeta del proyecto donde se encuentran los clientes.
3
Luego de movernos hasta la carpeta en la terminal, procedemos a compilar los
clientes utilizando el comando del compilador GCC:
gcc httpclient.c -o httpclient
Esto nos crea un ejecutable del cliente, podemos ejecutarlo utilizando el comando:
./httpclient localhost [port]
Tras utilizar ese comando el cliente se conectara con el servidor y nos mostrara
en la terminal los datos del servidor.
Adicionalmente tenemos un cliente desarrollado en python y que a trav´es de la
biblioteca [curl] nos permite conectarnos al servidor y obtener de igual forma
los datos principales. Para esto en la misma terminal y en la misma carpeta de
Clientes procedemos a escribir el comando:
python httpclient.py.
Por ´ultimo tenemos en la misma carpeta un archivo llamado StressCMD.c
que necesita ser compilado de igual forma utilizando el comando:
gcc StressCMD.c -o StressCMD
Tras compilarlo podemos ejecutarlo utilizando el comando:
StressCMD [cantidad-hilos] httpclient [parametros del cliente].
Esto nos permite bombardear el servidor con solicitudes para verificar que
seamos capaces de atender todas las entradas o requests que le hacemos al servidor.
NOTA:
Para el correcto funcionamiento del proyecto es necesario que se modifiquen dos
lineas de codigo:
La primera se encuentra en la linea 56 del archivo httpclient.c en la funci´on [void
Get()] debemos escribir la ruta donde localizamos la carpeta Files del proyecto
como se menciono anteriormente.
La segunda se encuentra en el archivo AttendRequest.c que se encuentra en
la carpeta de los servidores. Ah´ı es necesario modificar la l´ınea 8 para de igual
forma escribir la ruta donde encontramos la misma carpeta Files mencionada
anteriormente.
