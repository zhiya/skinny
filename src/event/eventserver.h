#ifndef __Include_event_eventserver_H__
#define __Include_event_eventserver_H__

#include "../server.h"

class Handler;

class EventServer : public Server{
public:
    EventServer(Server::Type type,unsigned short port,unsigned int maxConn=100);
    virtual ~EventServer();

    virtual bool start(Server::Service service);
    virtual void shutdown();

    virtual void notifySockDisconn(int skt) = 0;

protected:
    Handler *handler;
    unsigned int maxConn;

};

#endif//__Include_event_eventserver_H__

