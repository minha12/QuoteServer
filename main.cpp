#include <iostream>
#include <string>

#include "TcpListener.h"
#include "Quote.h"
#include <typeinfo>

using namespace std;

void Listener_MessageReceived(CTcpListener* listener, int client, string msg);

// This is kinda bad because it's global.
Quote quotes("wisdom.txt");

int main()
{
	CTcpListener server("127.0.0.1", 54010, Listener_MessageReceived);
    server.Run();
    return 0;
}

void Listener_MessageReceived(CTcpListener* listener, int client, string msg)
{
    /* Waring: for linux system, two hidden characters '\r\n' will be added
    two msg. For example, if client send message : 'Quote', the actual message
    is 'Quote\r\n'. The msg here in this function has already been stripped down 
    the two hidden character
    */
    if(msg == "Quote") {
        listener->Send(client, quotes.GetRandomQuote());
    }   
}


