#include <iostream>
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

	// bind socket to IP address and port
	//     setup:
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000); // convert from litle to big endian
	//     bind:
	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		cout << "Can't bind socket: " << WSAGetLastError() << endl;
		
		return -2;
	}
	
	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);

	char buff[1024];

	// Enter loop
	while (true)
	{
		ZeroMemory(buff, 1024);

		// wait for message
		int bytesIn = recvfrom(in, buff, 1024, 0, (sockaddr*)&client, &clientLength);

		if (bytesIn == SOCKET_ERROR)
		{
			cout << "Error receiving from client: " << WSAGetLastError() << endl;
			
			continue;
		}

		// display messagae and infor from client
		char clientIP[256];
		ZeroMemory(clientIP, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);
		cout << "Receive from " << clientIP << ": " << buff << endl;
	}

	// close socket
	closesocket(in);

	// shutdown WinSock
	WSACleanup();

	return 0;
}