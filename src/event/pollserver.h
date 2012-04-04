#ifndef __Include_event_pollserver_H__
#define __Include_event_pollserver_H__

#include "eventserver.h"
#include <poll.h>

class PollServer : public EventServer{
public:
    PollServer(Server::Type type,unsigned short port,unsigned int maxConn=100);
    ~PollServer();

    bool start(Server::Service service);
    void shutdown();

    void notifySockDisconn(int skt);

private:
    bool addsock(int skt,int events);
    void removesock(int skt);

    pollfd *fds;
    unsigned int nfds;
};

#endif//__Include_event_pollserver_H__


