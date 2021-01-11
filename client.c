#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
  // pasaremos como argumento la dirección ip del servidor y el puerto al que nos queremos conectar
  if(argc > 2) {
    // Un puntero a caractér que será la direción IP
    const char * ip;
    // Un socket para el cliente, número de bytes para enviar y recibir y el puerto al que nos conectaremos
    int client_socket, numbytes, puerto;
    // Un buffer para el mensaje
    char buff[100];
    // tomamos el puerto indicado en command line y la dirección IP
    puerto = atoi(argv[2]);
    ip = argv[1];
    // la estrcutura que almacena la información del servidor al que nos queremos conectar
    struct sockaddr_in server;
    /**
     * Intentamos convertir el argumento en una direccion IP, pasando
     * protocolos de internet, la ip del servidor
     */
    if(inet_pton(AF_INET, argv[1], &server.sin_addr) <= 0) {
      printf("La IP %s no es válida\n", ip);
      return -1;
    }
    /**
     * Si todo sale bien, abrimos el socket del cliente, validando
     * la dirección del cliente haciendo un llamado a socket con los
     * protocolos de internet y creamos un socket de tipo TCP/IP
     */
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("No pude abrir el socket\n");
      return -2;
    }
    /**
     * Si pudimos abrir el socket, indicaremos a qué server nos
     * queremos conectar: utilizaremos la familia de protocolos:
     */
    server.sin_family = AF_INET;
    // utilizaremos el puerto resultante de convertir el argumento en una estructura
    server.sin_port = htons(puerto);
    // llenamos con ceros el string sin_zero
    bzero(&(server.sin_zero), 8);
    // Intentamos la conexión
    printf("Conectando a %s:%s\n", argv[1], argv[2]);
    /**
     * Con la función connect intentamos la conexión, apuntando el socket
     * del cliente hacia la dirección del servidor, indicando el tamaño
     * de la estructra
     */
    if((connect(client_socket, (struct sockaddr *)&server, sizeof(struct sockaddr))) == -1) {
      perror("No pude conectarme al servidor\n");
      return -3;
    }
    /**
     * Si todo salió bien, podemos empezar a recibir información
     * desde el servidor, entonces verificamos si hay información a recibir
     * con la función recv, pasando el socket del cliente, el buffer,
     * su tamaño y las flags
     */
    if((numbytes = recv(client_socket, buff, 100, 0)) == -1) {
      printf("Error en la lectura\n");
      return -4;
    }
    // si pudimos leer, ponemos un /0 al final del buffer
    buff[numbytes] = '\0';
    printf("El servidor envió el mensaje '%s'\n", buff);
    // cerramos el client socket
    shutdown(client_socket, 2);
  } else {
    printf("Por favor indique ip del servidor y puerto\n");
  }
  return 0;
}