#ifndef __Include_server_H__
#define __Include_server_H__

#include <stdio.h>
#include <memory.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "common.h"

class Server{
public:
    enum Type{
        enType_UDP,
        enType_TCP,
    };
    static int GetSockType(Type type);

    enum Service{
        enService_Echo,
        enService_Http,
        enService_Rap,
    };

public:
    Server(Server::Type type,unsigned short port,unsigned short backlog=5);
    virtual ~Server();

    virtual bool start(Service service);
    virtual void shutdown();

protected:
    int type;
    unsigned short port;
    unsigned short backlog;
    int reuseaddr;
    int listensock;
};

#endif//__Include_server_H__

