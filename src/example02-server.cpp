#include <iostream>
#include "../include/Socket.hpp"
#include "../include/Structures.hpp"

/*
   Title: example02-server.cpp
   Author: Specter

   Purpose: Main Example of basic socket usage for server-sided use.

   More Info: This file is a basic example of how to use the Socket class. It simply
   binds to the given address/port, and listens for clients and accepts them. It will
   also respond to "<ping />" packets with "<pong />". Client handling can be edited
   at the top of Socket.cpp in /src.

   Documentation: All documentation can be found in /docs
*/

int main()
{
  Socket sock;

  /*
      std::string host - Hostname or IP of the server
      std::string port - The port of specified host to bind to
  */

  std::string host = "0.0.0.0";
  std::string port = "13337";

  // First we must specify the type of socket we are running, since we're running
  // a server we'll use TYPE_SERVER
  sock.setSocketType(TYPE_SERVER);

  // Initiate the socket with specified host and port
  sock.init(host, port);

  // Listen for clients and handle them
  sock.listenSocket();
}
