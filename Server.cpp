#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include "common.h"


class server{
    int _listening_socket;
    void Listen(int sock_fd, sockaddr_in addr){
        std::cout<<inet_ntoa(addr.sin_addr)<<":"<<ntohs(addr.sin_port)<<std::endl;
        int buff ;
        if(!try_send(sock_fd, buff)) return;
        int msg;
        if(!try_recv(sock_fd, msg)) return;
        std::cout<<"Get message:"<<msg<<std::endl;
        int answer = 1;
        if(!try_send(sock_fd, answer)) return;
        //if(!try_recv(sock_fd, buff)) return;
    }
public:
    server(unsigned short port){
        _listening_socket = check(socket(AF_INET, SOCK_STREAM, 0));
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        check(bind(_listening_socket, (sockaddr*)&addr, sizeof(addr)));
    }

    void start(){
        check(listen(_listening_socket, 1));
        sockaddr_in addr{};
        socklen_t len = sizeof(addr);
        //int sock_fd = check(accept(_listening_socket, (sockaddr*)&addr, &len));
        while(true){
            int sock_fd = check(accept(_listening_socket, (sockaddr*)&addr, &len));
            std::cout<<"Connect from: "<<inet_ntoa(addr.sin_addr)<<std::endl;
            Listen(sock_fd, addr);
            close(sock_fd);
            std::cout<<"Disconnect from: "<<inet_ntoa(addr.sin_addr)<<std::endl;
        }
    }
};

int main(){
    server ser(PORT);
    ser.start();
}