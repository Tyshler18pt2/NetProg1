#include <iostream>
#include <cstdlib>
#include <cstring>//line ��
#include <unistd.h> // for close()
//#include
#include <netinet/in.h>// sock_addr_in and allthing function  for network
#include <arpa/inet.h>
#include <sstream>
#include <string>
using namespace std;
void errors(const char* why, const int exitCode = 1)
{
    cerr << why << endl;
    exit(exitCode);
}
int main(int argc, char** argv)
{

    // ���������� ��������� � ������� ����� ��������� (�������)
    sockaddr_in* selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET; //�������� �������� IPv4
    selfAddr->sin_port = 0;         // ��� ��������������� ������� ���� ������� ����
    selfAddr->sin_addr.s_addr = 0;  // ��� ������ ����������
    // ��� ������ ������ ������ 0,����� ������� ���� �������� ������� �� �����
    // ���������� ��������� � ������� �������
    sockaddr_in* remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET;
    remoteAddr->sin_port = htons(7);
    remoteAddr->sin_addr.s_addr = inet_addr("95.83.72.118");//127.0.0.1

    // ���������� ������ ��� �������� � ������ ������
    char* buf = new char[256];
    strcpy(buf, "How are you?");
    int msgLen = strlen(buf); // ���������� ����� ������

    // �������� ������
    int mySocket = socket(AF_INET, SOCK_STREAM, 0); //TCP
    if (mySocket == -1) {
        errors("Error open socket", 11);
    }
    // �������� ������ � ������ ������
    int rc = bind(mySocket, (const sockaddr*)selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errors("Error bind socket with local address", 12);
    }
    // ��������� ���������� � ��������
    rc = connect(mySocket, (const sockaddr*)remoteAddr/*�����*/, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errors("Error bind socket with remote address", 13);
    }
    // �������� ������
    rc = send(mySocket, buf, msgLen, 0); // ���������� ����������� ���������� ������
    if (rc == -1) {
        close(mySocket);
        errors("Error send message", 14);
    }
    cout << "We send: " << buf << endl;
    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1) {
        close(mySocket);
        errors("Error recevie answer", 15);
    }
    buf[rc] = '\0';
    cout << "answer: " << buf << endl;
    // �������� ������
    close(mySocket);
    // ������������ ������
    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    return 0;
}