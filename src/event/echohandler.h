#ifndef __Include_event_echohandler_H__
#define __Include_event_echohandler_H__

#include <map>
#include <string>
#include <algorithm>

#include "handler.h"

class EchoHandler : public Handler{
public:
    EchoHandler(EventServer *server);
    virtual ~EchoHandler();

    virtual void read(int skt);
    virtual void write(int skt);

protected:
    struct PeerData{
        std::string name;
        std::string message;
    };
    PeerData *push(int skt,const std::string &msg);
    PeerData *get(int skt);
    bool has(int skt);
    void pop(int skt);

private:
    typedef std::map<int,PeerData*> defEchoMap;
    defEchoMap data;
};

#endif//__Include_event_echohandler_H__

