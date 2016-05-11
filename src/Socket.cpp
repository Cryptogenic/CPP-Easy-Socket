/*
   Title: Socket.cpp
   Author: Cryptogenic (Specter)
   Purpose: Contains implementation for Socket class. Documentation for class
            can be found in the header file (Socket.cpp) or in /docs.
*/

#include "../include/Socket.hpp"

using namespace std;

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
  /* Sets port number and sets up the socket with specified settings */
  portNumber = atoi(port.c_str());
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  /* Infinitely loop so we can break out on an error to avoid preforming sequential
     steps which won't be possible due to the error */
  for(;;)
  {
    if(sockfd < 0)
    {
      throwError("The socket failed to initialize");
      break;
    }

    server = gethostbyname(host.c_str());

    if(server == NULL)
    {
      throwError("The socket failed to find host: " + host);
      break;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portNumber);

    if(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
      throwError("The socket failed to connect to " + host + ":" + port);
      break;
    }
    break;
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

  Returns: Nothing (void)
*/

void Socket::throwError(string error)
{
  cout << "[SOCKET]\tError: " << error << endl;
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

  bzero(sendBuffer, buffer.length() + 1);     // Zero out all bytes for buffer

  memcpy(sendBuffer, buffer.c_str(), buffer.length());   // Copy given buffer into send buffer

  n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));

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
  bzero(buffer, MAX_RECEIVE_SIZE);            // Zero out all bytes for buffer

  n = read(sockfd, buffer, MAX_RECEIVE_SIZE - 1);

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
  close(sockfd);  // Close the socket and free memory
}
