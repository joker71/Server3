#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<string>
#include<sstream>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512

using namespace std;

int  aCharacter(string s)
{
	int res = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == 'A' || s[i] == 'a')
			res++;
	}
	return res;
}

int main() {
	int iResult = 0;

	/*****************************************************/
	/*    Khoi tao Winsock 2.2
	/*****************************************************/
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "\nWINSOCK: Khoi tao that bai\n";
		WSACleanup();
		return 0;
	}
	else {
		cout << "\nWINSOCK: Khoi tao thanh cong\n";
	}
	/*****************************************************/
	/*    Creating a Socket for the Server: ListenSocket */
	/*****************************************************/
	SOCKET ListenSocket = INVALID_SOCKET;
	// Create a SOCKET for the server to listen for client connections
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	/*****************************************************/
	/*    Binding a Socket*/
	/*****************************************************/
		//Xac dinh dinh danh ung dung 
	u_short port = 81;
	const char* IP = "127.0.0.1";
	SOCKADDR_IN svrAddr;
	int svrAddrLen = sizeof(svrAddr);
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(port);
	svrAddr.sin_addr.s_addr = inet_addr(IP);
	//inet_pton();
	cout << inet_addr(IP) << endl;;
	// Setup the TCP listening socket
	iResult = bind(ListenSocket, (SOCKADDR*)&svrAddr, svrAddrLen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	cout << "ListenSocket: " << ListenSocket << endl;

	while (1) {
		/*****************************************************/
		/*    Listening on a Socket*/
		/*****************************************************/

		if (listen(ListenSocket, 5) == SOCKET_ERROR) {
			printf("Listen failed with error: %ld\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		cout << "Listening ... " << endl;
		/*****************************************************/
		/*    Accepting a Connection*/
		/*****************************************************/
			//Create a temporary SOCKET object called ClientSocket for accepting connections from clients.
		SOCKET ClientSocket;
		ClientSocket = INVALID_SOCKET;

		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		cout << "ClientSocket: " << ClientSocket << endl;
		/*****************************************************/
		/*    Receiving and Sending Data on the Server*/
		/*****************************************************/

		char recvbuf[DEFAULT_BUFLEN];
		int sendreslen = 0;
		int iSendResult;
		int recvbuflen = DEFAULT_BUFLEN;
		int a[3];
		int index = 0;
		// Receive until the peer shuts down the connection
		do {

			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);


			if (iResult > 0) 
			{
				{
					int res;
					string sendRes;
					recvbuf[iResult] = '\0';
					stringstream geek(recvbuf);
					geek >> a[index];
					cout << "So thu " << index + 1 << ": " << a[index] << endl;
					index++;
					printf("Bytes received: %d\n", iResult);
					if (index == 2) {
						res = a[0] + a[1];
						cout << "Ket qua: " << res << endl;
						ostringstream oss;
						oss << res;
						sendRes = oss.str();
						cout << "Chuyen doi: " << sendRes << endl;
						iSendResult = send(ClientSocket, sendRes.c_str(), sizeof(sendRes), 0);
						if (iSendResult == SOCKET_ERROR) {
							printf("send failed: %d\n", WSAGetLastError());
							closesocket(ClientSocket);
							WSACleanup();
							return 1;
						}
						printf("Bytes sent: %d\n", iSendResult);
						index = 0;
						//choice = 0;
					}
				}
			}
			else if (iResult == 0)
				printf("Connection closing...\n");
			else {
				printf("recv failed: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}

		} while (iResult > 0);
		/*****************************************************/
		/*    Disconnecting the Server*/
		/*****************************************************/
			// shutdown the send half of the connection since no more data will be sent
		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
		// cleanup
		closesocket(ClientSocket);
	}//while(1){
	closesocket(ListenSocket);
	WSACleanup();


	return 0;
}


