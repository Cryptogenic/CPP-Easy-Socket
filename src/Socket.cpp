/*
   Title: Socket.cpp
   Author: Specter

   Purpose: Contains implementation/definitions for Socket class. Documentation
            for class can be found in the header file (Socket.cpp) or in /docs.
*/

#include <cassert>
#include <algorithm>
#include <string>

#include "../include/Socket.hpp"
#include "../include/Structures.hpp" // Contains SOCKET_TYPES structure

using namespace std;

/* Function: handleClient
   The function that handles clients as they're accepted. Should be modified
   to program's needs, hence why it is at the top

   Arguments:
    int sock - The socket reference

    Returns: Nothing (void)
*/

void Socket::handleClient(int sock)
{
  // Holds status code for read/write operation and stores received information
  int clientConnStatus;
  char clientBuffer[MAX_RECEIVE_SIZE];

  // Keep parsing information from the client until they disconnect
  do
  {
    memset(clientBuffer, '\0', MAX_RECEIVE_SIZE); // Zero out all bytes for buffer

    clientConnStatus = read(sock, clientBuffer, (MAX_RECEIVE_SIZE - 1));

    // Convert to a std::string
    string recvPacket(clientBuffer);

    // We have to strip the EOL that may be sent
    // Credit: Luke < http://stackoverflow.com/users/16434/luke >
    recvPacket.erase(remove(recvPacket.begin(), recvPacket.end(), '\n'), recvPacket.end());
    // End Credit

    if(clientConnStatus < 0)
    {
      throwError("We failed to read from a client! Terminating connection...", strerror(errno));
      return;
    }

    // For some reason, bugs will occur if a string literal and the string "recvPacket"
    // are in the same cout, so they are seperated.
    cout << "[RECV]\t\t";
    cout << recvPacket << endl;

    // If we received a "PING" packet, respond with "PONG" to show the server <-> client
    // connection is OK
    if(recvPacket.find("<ping />") != string::npos)
    {
      cout << "[SENT]\t\t<pong />" << endl;
      clientConnStatus = write(sock, "<pong />", 8);
    }

    if(clientConnStatus < 0)
    {
      throwError("We failed to write data to the client! Terminating connection...", "None");
      return;
    }
  } while(clientConnStatus != 0);
}

/*
  Function: Socket [CONSTRUCTOR]
  Class constructor, sets default active status code to true (1)

  Arguments: None

  Returns: Nothing (void)
*/

Socket::Socket()
{
  status = true;  // Everything is OK
}

/*
  Function: ~Socket [DECONSTRUCTOR]
  Class deconstructor, cleans up and unsets the socket

  Arguments: None

  Returns: Nothing (void)
*/

Socket::~Socket()
{
  close(sockfd);  // Destroy the socket object and free memory
}

/*
   Function: setSocketType
   Sets the socket type variable

   Arguments:
    int type - Either TYPE_CLIENT or TYPE_SERVER

  Returns: Nothing (void)
*/

void Socket::setSocketType(int type)
{
  clientSock = true;

  if(type == TYPE_SERVER)
    clientSock = false;
}

/*
  Function: init
  Initializes a socket with given parameters

  Arguments:
    string host   - Contains the host to connect to (IP Address)
    string port   - Contains the port to connect to on the given host

  Returns: Nothing (void)
*/

void Socket::init(string host, string port)
{
  // Sets port number and sets up the socket with specified settings
  portNumber = atoi(port.c_str());
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // Infinitely loop so we can break out on an error to avoid preforming sequential
  // steps which won't be possible due to the error
  for(;;)
  {
    if(sockfd < 0)
    {
      throwError("The socket failed to initialize", strerror(errno));
      break;
    }

    server = gethostbyname(host.c_str());

    if(server == NULL)
    {
      throwError("The socket failed to find host: " + host, "None");
      break;
    }

    memset(((char *)&serv_addr), '\0', (sizeof(serv_addr)));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNumber);

    // This only needs to be performed if the socket is for client-sided use
    if(clientSock)
    {
      memmove((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);

      if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
      {
        throwError("The socket failed to connect to " + host + ":" + port, strerror(errno));
        break;
      }
    }

    break; // Everything succeeded, break out of loop
  }
}

/*
  Function: writeSocket
  Writes the given buffer (argument) to the socket

  Arguments:
    string buffer  - Contains data to write to socket
    bool debugOutput - If true, print out what is being written to console

  Returns: Nothing (void)
*/

void Socket::writeSocket(string buffer, bool debugOutput)
{
  char sendBuffer[buffer.length() + 1];       // Make sure to include space for null terminator

  memset(sendBuffer, '\0', (buffer.length() + 1)); // Zero out all bytes for buffer
  memcpy(sendBuffer, buffer.c_str(), buffer.length());   // Copy given buffer into send buffer

  cout << sendBuffer << endl;

  n = write(sockfd, sendBuffer, strlen(buffer.c_str()));

  if(debugOutput)
    cout << "[SOCKET]\tPacket Written: " << buffer << endl;

  if (n < 0)
    throwWarning("We failed to write the following data to the socket;\n\t\t " + buffer);
}

/*
  Function: readSocket
  Reads from the socket and returns data read

  Arguments: None

  Returns:
    string buffer   - Returns the data read from the socket
    bool debugOutput - If true, print out what is being written to console
*/

string Socket::readSocket(bool debugOutput)
{
  memset(buffer, '\0', MAX_RECEIVE_SIZE); // Zero out all bytes for buffer

  n = read(sockfd, buffer, MAX_RECEIVE_SIZE - 1); // Subtract 1 to ensure room for null terminator

  if (n < 0)
  {
    throwWarning(strerror(errno));
    return "[ERR]";
  }

  string retBuffer(buffer);

  if(debugOutput)
    cout << "[SOCKET]\tPacket Read: " << retBuffer << endl;

  return retBuffer;
}

/*
  Function: listenSocket
  Binds to the host address, listens for clients and handles them

  Arguments: None

  Returns: Nothing (void)
*/

void Socket::listenSocket()
{
  if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    throwError("The socket could not be bound to port!", strerror(errno));
  }

  // MAX_BACKLOG_CONN = Maximum Backlog Connections, defined in Socket.hpp
  listen(sockfd, MAX_BACKLOG_CONN);
  clientLen = sizeof(cli_addr);

  // Infinite loop to accept multiple connections
  for(;;)
  {
    // Accept connections
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clientLen);

    if(newsockfd < 0)
    {
      throwError("We failed to accept a client!", strerror(errno));
      break; // Break out of the loop and terminate program
    }

    // Create a child process to handle individual client
    pid = fork();

    if(pid < 0)
    {
      throwError("We failed to create a child process to handle client", strerror(errno));
      break; // Break out of the loop and terminate program
    }

    if(pid == 0)
    {
      // Handle client
      close(sockfd);
      handleClient(newsockfd);
      break; // Break out of the loop and terminate program
    }
    else
    {
      close(newsockfd);
    }
  }
}
/*
  Function: checkStatus
  Returns the active status code as an integer

  Arguments: None

  Returns:
    int status  - Active status code
*/

bool Socket::checkStatus()
{
  return status;
}

/*
  Function: throwError
  Prints given error (argument) and sets active status code

  Arguments:
    string error  - Contains specified error message to output
    string moreInfo - Contains detailed information about the error via errno()

  Returns: Nothing (void)
*/

void Socket::throwError(string error, string moreInfo)
{
  cout << "[SOCKET]\tError: " << error << "\n\t\tMore Information: " << moreInfo << endl;
  status = false;
}

/*
  Function: throwWarning
  Prints given warning (argument), but does not set the active status code

  Arguments:
    string warning  - Contains specified warning message to output

  Returns: Nothing (void)
*/

void Socket::throwWarning(string warning)
{
  cout << "[SOCKET]\tWarning: " << warning << endl;
}
