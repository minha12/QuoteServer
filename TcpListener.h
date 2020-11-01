#pragma once

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#define MAX_BUFFER_SIZE (49152)

// Forward declaration of class
class CTcpListener;

// Callback to data received
typedef void(*MessageRecievedHandler)(CTcpListener* listener, int socketId, std::string msg);

class CTcpListener
{

public:

	// Constructor
	CTcpListener(std::string ipAddress, int port, MessageRecievedHandler handler);
	~CTcpListener();

	// Send a message to the specified client
	void Send(int clientSocket, std::string msg);

	// The main processing loop
	void Run();


private:

	// Create a socket
	int CreateSocket();

	// Wait for a connection
	int WaitForConnection(int listening);

	// Address of the server
	std::string				m_ipAddress;

	// Listening port
	int						m_port;

	// Message received event handler
	MessageRecievedHandler	MessageReceived;
};
