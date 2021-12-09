#include <iostream>
#include <string>
#include <WS2tcpip.h>

using namespace std;

int main()
{
	// startup WinSock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOK = WSAStartup(version, &data);
	if (wsOK != 0)
	{
		cout << "Can't start WinSock..\n";

		return -1;
	}

	// create hint structure for server
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(54000);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	// socket creation
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	// loop for sending message tests
	while (true)
	{
		// write out to the socket
		string s;

		cout << "send to server: ";
		getline(cin, s);

		if (s == "quit")
		{
			// quit from loop
			break;
		}

		int sendOK = sendto(out, s.c_str(), s.length() + 1, 0, (sockaddr*)&server, sizeof(server));
		if (sendOK == SOCKET_ERROR)
		{
			cout << "That didn't work: " << WSAGetLastError() << endl;
		}
	}

	// close socket
	closesocket(out);

	// shutdown WinSock
	WSACleanup();

	return 0;
}