#include "httphandler.h"
#include "selectserver.h"

HttpHandler::HttpHandler(EventServer *server)
: Handler(server)
{
    request_str = "";
}

HttpHandler::~HttpHandler(){
}

void HttpHandler::read(int skt){
    if(request_str.length()>0)return ;
    char tmpstr[4086];
    memset(tmpstr,0,sizeof(tmpstr));
    if(recv(skt,tmpstr,sizeof(tmpstr),0)<=0){
        printf("http receive end.\n");
        return ;
    }
    request_str = tmpstr;
    printf("http receive:\n%s\n",tmpstr);
}

void HttpHandler::write(int skt){
    if( request_str.length()<=0 )return ;
    const int tmplen = 4096;
    char otmp[tmplen];
    char msg[] = 
        "<html>"
        "<head>"
            "<title>"
                "TinyHttp Homesite!"
            "</title>"
        "</head>"
        "<body>"
        "<br/><br/><h2>Welcome to TinyHTTP Site!</h2>"
        "<p>I wrote it @linux with c++.It's so cool!!!</p>"
        "</body>"
        "</html>";
    int msglen = strlen(msg);
    sprintf(otmp,
            "HTTP/1.1 200 OK\r\n"
            "Server: TinyCGI/0.0.1\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %u\r\n\r\n"
            "%s",msglen,msg);
    if(send(skt,otmp,tmplen,0)<=0){
        printf("http send failed!\n");
    }
    request_str = "";
    server->notifySockDisconn(skt);
}


