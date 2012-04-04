#ifndef __Include_event_handler_H__
#define __Include_event_handler_H__

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>

class EventServer;

class Handler{
public:
    Handler(EventServer *server):server(server){};
    virtual ~Handler(){};

    virtual void read(int skt) = 0;
    virtual void write(int skt) = 0;

protected:
    EventServer *server;
};

#endif//__Include_event_handler_H__


