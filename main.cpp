#include <iostream>
#include <string>

#include "TcpListener.h"
#include "Quote.h"

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
    // cout << "msg at main: " << msg << endl;
    // cout << "msg at main: " << quotes.GetRandomQuote() << endl;
    cout << "check!" << endl;
    //if(msg == "Q") {
    if("Q" == msg) {
        listener->Send(client, quotes.GetRandomQuote());
    }   
}


