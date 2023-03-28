#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define BUFSIZE         1024   
#define SERVER_PORT     54000   

int main(void)
{
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(MAKEWORD(2, 2), &data);
    if (wsResult != 0)
    {
        cerr << "WSAStartup failed, Err #" << wsResult << endl;
        cin.get();
        return 0;
    }

    //Создание сокета
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create a socket, Err #" << WSAGetLastError() << endl;
        cin.get();
        WSACleanup();
        return 0;
    }

    int OptVal = 1;
    int OptLen = sizeof(int);
    int rc = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&OptVal, OptLen);
    if (rc == SOCKET_ERROR)
    {
        cerr << "Can't set a sockopt, Err #" << WSAGetLastError() << endl;
        cin.get();
        WSACleanup();
        return 0;
    }
    else
        cout << "Socket option SO_REUSEADDR is ON" << endl;

    //Запрос на ввод IP сервера
    cout << "IP adress of server : ";
    string ipAdress;
    getline(cin, ipAdress);

    //Структура сервера и клиента
    sockaddr_in server_addr, client_addr;

    int server_len = sizeof(server_addr);
    ZeroMemory(&server_addr, server_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, ipAdress.c_str(), &server_addr.sin_addr);

    int client_len = sizeof(client_addr);
    ZeroMemory(&client_addr, client_len);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(0);
    client_addr.sin_addr.S_un.S_addr = INADDR_ANY;

    rc = bind(sock, (struct sockaddr*)&client_addr, client_len);
    if (rc == SOCKET_ERROR)
    {
        cerr << "Can't bind a socket, Err #" << WSAGetLastError() << endl;
        cin.get();
        WSACleanup();
        return 0;
    }
    char buf[BUFSIZE];
    string userInput;

    do
    {
        //Запрос на ввод текста
        cout << "> ";
        getline(cin, userInput);

        if (userInput.size() > 0)
        {
            //Отправка текста
            int sendResult = sendto(sock, userInput.c_str(), BUFSIZE, 0, (sockaddr*)&server_addr, server_len);
            if (sendResult == SOCKET_ERROR)
            {
                cerr << "Can't send msg, Err #" << WSAGetLastError() << endl;
                cin.get();
                return 0;
            }
           
        }
    } while (userInput.size() > 0);
    cin.get();
    closesocket(sock);
    WSACleanup();
    return 0;
}
