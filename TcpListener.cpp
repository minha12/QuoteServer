#include "TcpListener.h"

CTcpListener::CTcpListener(std::string ipAddress, int port, MessageRecievedHandler handler)
	: m_ipAddress(ipAddress), m_port(port), MessageReceived(handler)
{

}

// Send a message to the specified client
void CTcpListener::Send(int clientSocket, std::string msg)
{
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}


CTcpListener::~CTcpListener()
{
}

// The main processing loop
void CTcpListener::Run()
{
	char buf[MAX_BUFFER_SIZE];

	while (true)
	{
		// Create a listening socket
		int listening = CreateSocket();
		if (listening == -1)
		{
			break;
		}

		int client = WaitForConnection(listening);
		if (client != -1)
		{
			close(listening);

			int bytesReceived = 0;
			do
			{
				memset(buf, 0, MAX_BUFFER_SIZE);

				bytesReceived = recv(client, buf, MAX_BUFFER_SIZE, 0);
				
				if (bytesReceived > 0)
				{
					if (MessageReceived != NULL)
					{
						/* By default in linux system, when client sends message, two
						character '\r\n' will be added at the end of the message, this
						will cause trouble in the main function, i.e. Listener_MessageReceived 
						function, if we compare the received text with an expected test, i.e., 
						if(expected_text == msg), even though two texts look the same, the condtion 
						of the if statement always false, so in the code below we will remove 
						those two characters */
						if (buf[strlen(buf)-1] == '\n' && buf[strlen(buf)-2] == '\r') {
							buf[strlen(buf)-1] = '\0'; // rm '\n'
							buf[strlen(buf)-1] = '\0'; // rm '\r'
						}
						std::cout << "CLIENT> " << buf << std::endl;
						MessageReceived(this, client, buf);
					}
				}

			} while (bytesReceived > 0);

			close(client);
		}
		close(client);
	}
}

// Create a socket
int CTcpListener::CreateSocket()
{
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening != -1)
	{
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_port);
		inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

		int bindOk = bind(listening, (sockaddr*)&hint, sizeof(hint));
		if (bindOk != -1)
		{
			int listenOk = listen(listening, SOMAXCONN);
			if (listenOk == -1)
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}

	return listening;
}

// Wait for a connection
int CTcpListener::WaitForConnection(int listening)
{
	int client = accept(listening, NULL, NULL);
	return client;
}
