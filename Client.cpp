#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <iostream>
#include "common.h"

class Client{
    int _socket;
    void Send(){
        struct ucred cred;
        cred.uid = getuid();
        int buff = cred.uid;
        //std::cout<<"UID: " << buff;
        if(!try_send(_socket, buff)) return;

        Request request;
        request.target_uid = buff;
        request.mode = S_IRUSR;
        strcpy(request.name, "test.txt");
        request.rights = R_READ;
        if(!try_send(_socket, request)) return;
    }
public:
    void start(int port){
        _socket = check(socket(AF_INET, SOCK_STREAM, 0));
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        check(connect(_socket, (sockaddr*)&addr, sizeof(addr)));

        Send();
        close(_socket);
    }
};

int main(){
    Client client;
    client.start(PORT);
}