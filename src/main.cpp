#include <iostream>
#include "../include/Socket.hpp"
#include "../include/Structures.hpp"

/*
   Title: main.cpp
   Author: Specter

   Purpose: Main Example of basic socket usage.

   More Info: This file is a basic example of how to use the Socket class. It simply
              attempts to connect and prints if a connection could be established.
              If a connection could be established, the program returns 0, otherwise,
              it returns -1.

   Documentation: All documentation can be found in /docs
*/

int main()
{
  Socket sock;

  /*
      std::string host - Hostname or IP of the server
      std::string port - The port of specified host to connect to (or bind to)
  */

  std::string host = "0.0.0.0";
  std::string port = "13337";

  //First we must specify the type of socket we are running, either a TYPE_CLIENT or TYPE_SERVER.
  sock.setSocketType(TYPE_CLIENT);

  // Initiate the socket with specified host and port
  sock.init(host, port);

  // If the socket is connected with no errors, return 0. Otherwise, return -1
  if(sock.checkStatus())
  {
    std::cout << "The connection was established successfully, terminating...\n";
    return 0;
  }
  else
  {
    std::cout << "The connection could not be established, terminating...\n";
    return -1;
  }
}
