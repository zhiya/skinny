#include <unistd.h>
#include <signal.h>
#include "basic/basicserver.h"
#include "event/selectserver.h"
#include "event/pollserver.h"

int test0(){
    BasicServer server(Server::enType_TCP,1986);
    return server.start(Server::enService_Http);
}

int test1(){
    SelectServer server(Server::enType_TCP,1987);
    return server.start(Server::enService_Http);
}

int test2(){
    PollServer server(Server::enType_TCP,1988);
    return server.start(Server::enService_Http);
}

int test3(){
    BasicServer s1(Server::enType_TCP,2012);
    SelectServer s2(Server::enType_TCP,2013);

    signal(SIGCHLD,SIG_IGN);

    pid_t p1=0,p2=0;
    if((p1=fork())==0){
        return s1.start(Server::enService_Echo);
    }
    if((p2=fork())==0){
        return s2.start(Server::enService_Http);
    }
    return 0;
}

//main entry
int main(int argc,char **argv){
    return test2();
}


