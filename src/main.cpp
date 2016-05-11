#include <iostream>
#include "../include/Socket.hpp"

/*
   Title: main.cpp
   Author: Cryptogenic (Specter)
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
      std::string port - The port of specified host to connect to
  */

  std::string host = "127.0.0.1";
  std::string port = "6667";

  /* Initiate the socket with specified host and port */
  sock.init(host, port);

  /* If the socket is connected with no errors, return 0. Otherwise, return -1 */
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
