#include "server.h"

int Server::GetSockType(Server::Type type){
    switch(type){
        case enType_UDP:
            return SOCK_DGRAM;
        case enType_TCP:
            return SOCK_STREAM;
        default:
            return SOCK_STREAM;
    }
}

Server::Server(Server::Type type,unsigned short port,unsigned short backlog)
    : type(type)
    , port(port)
    , backlog(backlog)
    , reuseaddr(1)
    , listensock(0)
{
}

Server::~Server(){
}

bool Server::start(Server::Service service){
    struct sockaddr_in addr = {AF_INET,htons(port),INADDR_ANY,0};
    if((listensock = ::socket(AF_INET,GetSockType((Type)type),0))<=0){
        printf("create listen socket failed!\n");
        return false;
    }
    if(::setsockopt(listensock,SOL_SOCKET,SO_REUSEADDR,&reuseaddr,sizeof(reuseaddr))){
        printf("setsockopt [SO_REUSEADDR] failed!\n");
        shutdown();
        return false;
    }
    if(::bind(listensock,(struct sockaddr*)&addr,sizeof(addr))){
        printf("bind listen socket failed!\n");
        shutdown();
        return false;
    }
    if(::listen(listensock,backlog)){
        printf("server listen failed!\n");
        shutdown();
        return false;
    }
    printf("tinycgi server's listening at %d\n",port);
    return true;
}

void Server::shutdown(){
    if(listensock){
        ::shutdown(listensock,SHUT_RDWR);
        listensock = 0;
    }
}


