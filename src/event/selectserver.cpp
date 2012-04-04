#include "selectserver.h"
#include "echohandler.h"
#include "httphandler.h"

SelectServer::SelectServer(Server::Type type,unsigned short port,unsigned int maxConn)
    : EventServer(type,port,maxConn)
    , maxfd(0)
    , socks(NULL)
    , rfds(NULL)
    , wfds(NULL)
    , efds(NULL)
{
}

SelectServer::~SelectServer(){
}

bool SelectServer::start(Service service){
    if(!EventServer::start(service)){
        return false;
    }
    socks = new int[maxConn];
    rfds = new fd_set;
    wfds = new fd_set;
    while(1){
        if(rfds)FD_ZERO(rfds);
        if(wfds)FD_ZERO(wfds);

        FD_SET(listensock,rfds);
        maxfd = maxfd>listensock?maxfd:listensock;
        for(int i=0;i<maxConn;++i){
            int skt = socks[i];
            if(skt > 0){
                FD_SET(skt,rfds);
                FD_SET(skt,wfds);
                maxfd = maxfd>skt?maxfd:skt;
            }
        }

        if(::select(maxfd+1,rfds,wfds,efds,NULL)>0){
            for(int i=0;i<maxConn;++i){
                int skt = socks[i];
                if(skt > 0){
                    if(FD_ISSET(skt,rfds)){
                        handler->read(skt);
                    }
                    if(FD_ISSET(skt,wfds)){
                        handler->write(skt);
                    }
                }
            }
            if(FD_ISSET(listensock,rfds)){
                //new comming
                struct sockaddr_in addr;
                memset(&addr,0,sizeof(addr));
                socklen_t addrlen = 0;
                int skt = ::accept(listensock,(struct sockaddr *)&addr,&addrlen);
                if( skt>0 ){
                    int i=0;
                    for(;i<maxConn;++i){
                        if(socks[i]<=0){
                            socks[i] = skt;
                            printf("accept new socket[%d]\n",skt);
                            break;
                        }
                    }
                    if(i==maxConn){
                        const char *msg="server overload,timeout!";
                        ::send(skt,msg,strlen(msg)+1,0);
                        ::shutdown(skt,SHUT_RDWR);
                    }
                }else{
                    printf("accept error!\n");
                }
            }
        }
    }
    return true;
}

void SelectServer::shutdown(){
    EventServer::shutdown();
    maxfd = 0;
    idelete(socks);
    idelete(rfds);
    idelete(wfds);
    idelete(efds);
}

void SelectServer::notifySockDisconn(int skt){
    if(skt<=0) return ;
    for(int i=0;i<maxConn;++i){
        if(socks[i]==skt){
            ::shutdown(skt,SHUT_RDWR);
            FD_CLR(skt,rfds);
            FD_CLR(skt,wfds);
            socks[i] = 0;
        }
    }
}


