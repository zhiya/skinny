#include "echohandler.h"
#include "selectserver.h"

EchoHandler::EchoHandler(EventServer *server)
    :Handler(server)
{
}

EchoHandler::~EchoHandler(){
}

void EchoHandler::read(int skt){
    char tmpstr[4096];
    memset(tmpstr,0,sizeof(tmpstr));
    if(recv(skt,tmpstr,sizeof(tmpstr),0)>0){
        PeerData *peer = this->push(skt,tmpstr);
        printf("[%d](%s): %s\n",skt,peer->name.c_str(),tmpstr);
    }else{
        this->pop(skt);
        server->notifySockDisconn(skt);
    }
}

void EchoHandler::write(int skt){
    PeerData *peer = this->get(skt);
    if(!peer||peer->message.length()<=0) return ;

    if(send(skt,peer->message.c_str(),peer->message.length()+1,0)<=0){
        printf("echo handler send data failed[%d,%s]!",skt,peer->message.c_str());
    }
    peer->message = "";
}

EchoHandler::PeerData *EchoHandler::push(int skt,const std::string &msg){
    if( skt<=0 ) return NULL;
    defEchoMap::iterator res=data.find(skt);
    if(res==data.end()){
        PeerData *peer = new PeerData();
        peer->name = msg;
        peer->message = "you are welcome!";
        data[skt] = peer;
        return peer;
    }else{
        res->second->message = msg;
        return res->second;
    }
    //printf("recv[%d]:%s\n",skt,msg.c_str());
}

EchoHandler::PeerData *EchoHandler::get(int skt){
    defEchoMap::iterator res=data.find(skt);
    if(res!=data.end()){
        return res->second;
    }
    //printf("send[%d]:%s\n",skt,msg.c_str());
    return NULL;
}

bool EchoHandler::has(int skt){
    return data.find(skt)!=data.end();
}

void EchoHandler::pop(int skt){
    defEchoMap::iterator res=data.find(skt);
    if(res!=data.end()){
        delete res->second;
        data.erase(res);
    }
}

