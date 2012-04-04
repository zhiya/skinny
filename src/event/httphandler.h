#ifndef __Include_event_httphandler_H__
#define __Include_event_httphandler_H__

#include <string>
#include "handler.h"

class HttpHandler : public Handler{
public:
    HttpHandler(EventServer *server);
    virtual ~HttpHandler();

    virtual void read(int skt);
    virtual void write(int skt);

private:
    std::string request_str;
};

#endif//__Include_event_httphandler_H__

