#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>

#define BUF_SIZE 100

//Obtenemos la informacion del servidor
struct addrinfo *getHostInfo(char* host, char* port) {
  int r;
  struct addrinfo hints, *getaddrinfo_res;
  // Setup hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if ((r = getaddrinfo(host, port, &hints, &getaddrinfo_res))) {
    fprintf(stderr, "[getHostInfo:21:getaddrinfo] %s\n", gai_strerror(r));
    return NULL;
  }

  return getaddrinfo_res;
}

//Establecemos conexion con el servidor
int establishConnection(struct addrinfo *info) {
  if (info == NULL) return -1;

  int clientfd;
  for (;info != NULL; info = info->ai_next) {
    if ((clientfd = socket(info->ai_family,
                           info->ai_socktype,
                           info->ai_protocol)) < 0) {
      perror("[establishConnection:35:socket]");
      continue;
    }

    if (connect(clientfd, info->ai_addr, info->ai_addrlen) < 0) {
      close(clientfd);
      perror("[establishConnection:42:connect]");
      continue;
    }

    freeaddrinfo(info);
    return clientfd;
  }

  freeaddrinfo(info);
  return -1;
}

//Peticion GET
void GET(int clientfd) {
  char req[1000] = {0};
  sprintf(req, "GET %s HTTP/1.0\r\n\r\n","/home/wal/Escritorio/Operativos/server/WebServerC-master2/Files");
  send(clientfd, req, strlen(req), 0);
}

int main(int argc, char **argv) {
  int clientfd;
  char buf[BUF_SIZE];

  if (argc != 3) {
    fprintf(stderr, "USAGE: ./httpclient <hostname> <port>\n");
    return 1;
  }

  //Establecemos la conexion
  clientfd = establishConnection(getHostInfo(argv[1], argv[2]));
  if (clientfd == -1) {
    fprintf(stderr,
            "[main:73] Failed to connect to: %s:%s%s \n",
            argv[1], argv[2], argv[3]);
    return 3;
  }

  //Enviamos el get request
  GET(clientfd);
  while (recv(clientfd, buf, BUF_SIZE, 0) > 0) {
    fputs(buf, stdout);
    memset(buf, 0, BUF_SIZE);
  }

  close(clientfd);
  return 0;
}
