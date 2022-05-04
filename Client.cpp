#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include "common.h"

class Client{
    int _socket;
    void Send(){
        int msg;
        if(!try_recv(_socket, msg)) return;


        int len;
        struct ucred cred;
        cred.uid = getuid();
        //len = sizeof(cred);
        //check(getsockopt(_socket, SOL_SOCKET, SO_PEERCRED, &cred, &len));

        int buff = cred.uid;
        std::cout<<"Message: " << buff;
        //std::cin>>buff;
        if(!try_send(_socket, buff)) return;
        int answer;
        if(!try_recv(_socket, answer)) return;
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