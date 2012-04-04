#include "pollserver.h"
#include "echohandler.h"
#include "httphandler.h"

PollServer::PollServer(Server::Type type,unsigned short port,unsigned int maxConn)
    : EventServer(type,port,maxConn)
    , fds(NULL)
    , nfds(0)
{
}

PollServer::~PollServer(){
}

bool PollServer::start(Server::Service service){
    if(!EventServer::start(service)){
        return false;
    }

    fds = new pollfd[maxConn];
    memset(fds,0,sizeof(pollfd)*maxConn);

    addsock(listensock,POLLIN);
    while(1){
        if(::poll(fds,nfds,-1)>0){
            for(unsigned int i=1;i<nfds;++i){
                if(fds[i].fd<=0) continue;
                if(fds[i].revents&POLLIN){
                    handler->read(fds[i].fd);
                }
                if(fds[i].revents&POLLOUT){
                    handler->write(fds[i].fd);
                }
            }
            if(fds[0].revents&POLLIN){
                //new connection
                struct sockaddr_in addr;
                memset(&addr,0,sizeof(addr));
                socklen_t addrlen=0;
                int skt = 0;
                if((skt=::accept(fds[0].fd,(struct sockaddr *)&addr,&addrlen))>0){
                    if(addsock(skt,POLLIN|POLLOUT)){
                        printf("new client[%d]\n",skt);
                    }else{
                        printf("client accept failed!\n");
                    }
                }else{
                    printf("accept failed!\n");
                }
            }
        }
    }

    return true;
}

void PollServer::shutdown(){
    EventServer::shutdown();
    idelete(fds);
    nfds = 0;
}

void PollServer::notifySockDisconn(int skt){
}

bool PollServer::addsock(int skt,int events){
    if(nfds<maxConn){
        fds[nfds].fd = skt;
        fds[nfds].events = events;
        nfds++;
        return true;
    }
    return false;
}

void PollServer::removesock(int skt){
    bool found = false;
    for(unsigned int i=0;i<nfds;++i){
        if(fds[i].fd == skt){
            found = true;
        }
        if(!found) continue;
        if(i+1<nfds){
            memcpy(fds+i,fds+i+1,sizeof(pollfd));
        }else{
            memset(fds+i,0,sizeof(pollfd));
        }
    }
}



