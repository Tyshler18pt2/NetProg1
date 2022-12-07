#include <iostream>
#include <cstdlib>
#include <cstring>//line ÑÈ
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

    // ïîäãîòîâêà ñòðóêòóðû ñ àäðåñîì íàøåé ïðîãðàììû (êëèåíòà)
    sockaddr_in* selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET; //èíòåðíåò ïðîòîêîë IPv4
    selfAddr->sin_port = 0;         // ïðè âçàèìîäåéñòèâèè ñèñòåìà ñàìà âûáèðàåò ïîðò
    selfAddr->sin_addr.s_addr = 0;  // âñå àäðåñà êîìïüþòåðà
    // äëÿ ñâîåãî àäðåñà ëó÷øå óêàçàòü 0,÷òîáû ñèñòåìà âûáðàëà ñàìà 
    // ïîäãîòîâêà ñòðóêòóðû ñ àäðåñîì ñåðâåðà
    sockaddr_in* remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET;
    remoteAddr->sin_port = htons(13);
    remoteAddr->sin_addr.s_addr = inet_addr("95.83.72.118");

    // ïîäãîòîâêà áóôåðà äëÿ ïåðåäà÷è è ïðèåìà äàííûõ
    char* buf = new char[256];
    strcpy(buf, "Give me please time");
    int msgLen = strlen(buf); // âû÷èñëÿåì äëèíó ñòðîêè

    // ñîçäàíèå ñîêåòà
    int mySocket = socket(AF_INET, SOCK_STREAM, 0); //TCP
    if (mySocket == -1) {
        errors("Error open socket", 11);
    }
    // ïðèâÿçêà ñîêåòà ê ñâîåìó àäðåñó
    int rc = bind(mySocket, (const sockaddr*)selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errors("Error bind socket with local address", 12);
    }
    // óñòàíîâêà ñîåäèíåíèå ñ ñåðâåðîì
    rc = connect(mySocket, (const sockaddr*)remoteAddr/*àäðåñ*/, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errors("Error bind socket with remote address", 13);
    }
    // ïåðåäà÷à äàííûõ
    rc = send(mySocket, buf, msgLen, 0); // âîçâðàùàåò êîëè÷åñòîâî ïåðåäàííûõ áàéòîâ
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
    // çàêðûòèå ñîêåòà
    close(mySocket);
    // îñâîáîæäåíèå ïàìÿòè
    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    return 0;
}
