#include "eventserver.h"
#include "echohandler.h"
#include "httphandler.h"

EventServer::EventServer(Server::Type type,unsigned short port,unsigned int maxConn)
    : Server(type,port,5)
    , maxConn(maxConn)
{
}

EventServer::~EventServer(){
}

bool EventServer::start(Service service){
    if(!Server::start(service)){
        return false;
    }

    switch(service){
        case Server::enService_Echo:
            handler = new EchoHandler(this);
            break;
        case Server::enService_Http:
            handler = new HttpHandler(this);
            break;
        default:
            handler = new EchoHandler(this);
            break;
    }

    return true;
}

void EventServer::shutdown(){
    Server::shutdown();
    idelete(handler);
}


