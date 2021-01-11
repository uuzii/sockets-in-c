# Sockets
This is the most basic implementation of sockets in c

In order to test this project locally, you have to compile the server and then run it in some port by executing:

```bash
gcc server.c -o server
./server [port]
```

Then, you have to compile and run the client in another terminal by using:
```bash
gcc server client.c -o client
./client 127.0.0.1 [port]
```
