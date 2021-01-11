#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
  // pasaremos como argumento el puerto en el que está escuchando
  if(argc > 1) {
    // si existe el argumento, generamos las variables
    int server_socket, client_socket, puerto;
    unsigned int longitud_cliente;
    puerto = atoi(argv[1]);
    // definimos las direcciones de cliente y servidor
    struct sockaddr_in server;
    struct sockaddr_in client;
    // configuramos el protocolo a utilizar (TCP/IP)
    server.sin_family = AF_INET;
    // configuramos el puerto con el tipo de dato que acepta la librería
    server.sin_port = htons(puerto);
    // configuramos la dirección de quién puede conectarse (cualquiera)
    server.sin_addr.s_addr = INADDR_ANY;
    // rellenamos el string con 8 ceros para poder utilizar las librerías
    bzero(&(server.sin_zero), 8);
    /**
     * Intentamos crear el socket
     * Lo ponemos en modo escucha
     * Le pasamos: familia de protocolos a usar, el protocolo específico
     *  y la opción de configuración automática.
     */
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      // mandaremos un error por la salida de error
      perror("No pude abrir el socket\n");
      return -1;
    }
    /**
     * Aquí ya tendríamos el socket abierto:
     * Conectaremos este socket a un puerto con la función bind
     * Mandaremos un puntero a la estructura sock_addr
     * Guardaremos la configuración de bind en la variable server y
     * el tamaño de estructura
     */
    if(bind(server_socket, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
      // En el caso de obtener -1, lanzariamos error (puede estar ocupado ya)
      printf("No pude abrir el puerto %s\n", argv[1]);
      return -2;
    }
    /**
     * Si todo se dió bien, podemos poner todo esto en modo escucha,
     * donde configuraremos: el tamaño de la cola (clientes atendidos de
     * manera simultánea)
     */
    if(listen(server_socket, 5) == -1) {
      // Lanzamos un error si la orden falla:
      perror("No pude ponerme en modo escucha\n");
      return -3;
    }
    /**
     * Para recibir y dar servicio a las peticiones de clientes,
     * Definiremos el tamaño del socket
     * Intentaremos recibir a nuestro primer cliente: aceptando una conexión
     * en nuestro server e indicando el puntero a la dirección del cliente
     */
    printf("Esperando clientes...\n");
    longitud_cliente = sizeof(struct sockaddr_in);
    if((client_socket = accept(server_socket, (struct sockaddr *) &client, &longitud_cliente)) == -1) {
      printf("No pudimos aceptar una conexión\n");
      return -4;
    }
    // Si todo funcionó correctamente iniciamos la comunicación, verificamos la dirección
    char str[INET_ADDRSTRLEN];
    /**
     * Transformamos la estrucutra de la dirección a un string:
     * Indicamos el protocolo, la dirección que queremos transformar, indicamos
     * en qué buffer la vamos a colocar y su tamaño máximo
     */
    inet_ntop(AF_INET, &(client.sin_addr), str, INET_ADDRSTRLEN);
    // Damos información de quién se conectó
    printf("Se conectó un cliente desde %s:%d. Lo saludo\n", str, puerto);
    /**
     * Enviamos un mensaje, su tamaño, y podemos enviar banderas que
     * proporcionarían información bit a bit
     */
    send(client_socket, "Bienvenido a mi servidor.\n", 26, 0);
    printf("Saludo enviado\n");
    // Cerramos sockets para cancelar la entrada/salida
    shutdown(client_socket, 2);
    shutdown(server_socket, 2);
  } else {
    printf("Por favor indique el puerto\n");
    return -5;
  }
  return 0;
}