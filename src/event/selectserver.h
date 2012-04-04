#ifndef __Include_event_selectserver_H__
#define __Include_event_selectserver_H__

#include "eventserver.h"
#include <list>

class SelectServer : public EventServer{
public:
    SelectServer(Server::Type type,unsigned short port,unsigned int maxConn=100);
    ~SelectServer();

    bool start(Server::Service service);
    void shutdown();

    void notifySockDisconn(int skt);

private:
    int maxfd;
    int *socks;
    fd_set *rfds;
    fd_set *wfds;
    fd_set *efds;
};

#endif//__Include_event_selectserver_H__

