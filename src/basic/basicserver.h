#ifndef __Include_basic_basicserver_H__
#define __Include_basic_basicserver_H__

#include "../server.h"

class BasicServer : public Server{
public:
    BasicServer(Server::Type type,unsigned short port);
    ~BasicServer();

    bool start(Server::Service service);
};

#endif//__Include_basic_basicserver_H__

