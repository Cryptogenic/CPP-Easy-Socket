/*
   Title: Socket.hpp
   Author: Specter

   Purpose: Contains function prototypes and required declarations for the Socket
            class.
*/

#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <winsock2.h>
#include <windows.h>
#else
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#endif

#include <string>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

const int MAX_RECEIVE_SIZE = 32768; // The maximum number of bytes we will accept
const int MAX_BACKLOG_CONN = 6; // The maximum number of connections to accept in queue

/*
  Class: Socket
  Allows a basic client socket to be opened and contains methods for data to
  be sent and received.

  Private Properties:
    int status      - Contains the active status code of the socket
    int sockfd      - Contains the reference to the socket
    int newsockfd   - Contains the reference to a new socket for accepting clients
    int portNumber  - Contains the port # for the socket as an integer
    int n           - Contains the status code of socket functions (read, write)
    int pid         - Contains program PID for forking
    int clientLen   - Contains size of cli_addr structure

    bool clientSock - Contains true or false bool if socket is for client connections

    char buffer     - Contains the data for reading

    struct sockaddr_in  - Contains the family, port, host, and zero for socket
    struct serv_addr    - Also contains family, port, and host
    struct cli_addr     - Also contains family, port, and host
    struct hostent      - Contains host name, aliases, address type, and length

  Public Methods:
    Socket()        - Constructor, sets status variable to default 1
    ~Socket()       - Deconstructor, cleans up and unsets the socket

    bool checkStatus()   - Returns the if the socket is alive or dead
    void setSocketType() - Sets the socket type (TYPE_CLIENT or TYPE_SERVER)
    void init()         - Initializes a socket with given parameters

    void writeSocket()          - Writes the given buffer (argument) to the socket
    std::string readSocket()    - Reads from the socket, returns data read as a string

    void handleClient() - Handles clients as they're accepted (at top of Socket.cpp)
    void listenSocket() - Binds to the host address, listens for clients and handles them

  Private Methods:
    void throwError()   - Prints given error (argument) and sets active status code
    void throwWarning() - Prints given warning, does not set active status code
*/
class Socket
{
public:
  Socket();
  ~Socket();

  bool checkStatus();
  void setSocketType(int type);
  void init(std::string host, std::string port);

  void writeSocket(std::string buffer, bool debugOutput = false);
  std::string readSocket(bool debugOutput = false);

  void handleClient(int sock);
  void listenSocket();

private:
  int status;
  int sockfd, newsockfd, portNumber, n, pid, clientLen;
  struct sockaddr_in serv_addr, cli_addr;
  struct hostent *server;
  char buffer[MAX_RECEIVE_SIZE];

  bool clientSock;

  void throwError(std::string error, std::string moreInfo);
  void throwWarning(std::string warning);
};

#endif
