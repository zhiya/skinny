#include "basicserver.h"
#include "handler.h"

BasicServer::BasicServer(Server::Type type,unsigned short port)
    : Server(type,port,5)
{
}

BasicServer::~BasicServer(){
}

bool BasicServer::start(Server::Service service){
    if(!Server::start(service)){
        return false;
    }

    defHandler handler = handle_echo;
    switch(service){
        case Server::enService_Echo:
            handler = handle_echo;
            break;
        case Server::enService_Http:
            handler = handle_http;
            break;
        case Server::enService_Rap:
            handler = handle_rap;
            break;
        default:
            handler = handle_echo;
            break;
    }

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    socklen_t addrlen= 0;
    while(1){
        handler(::accept(listensock,(struct sockaddr *)&addr,&addrlen));
    }

    return true;
}

