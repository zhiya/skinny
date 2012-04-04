#include "handler.h"
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../common.h"


void handle_rap(int skt){
    if(skt<=0){
        printf("accept client failed!\n");
        return ;
    }

    char name[64];
    memset(name,0,sizeof(name));
    recv(skt,name,sizeof(name),0);
    printf("accept client %s\n",name);

    if(fork()==0){
        char tmpstr[128];
        int rt = 0;
        while((rt=recv(skt,tmpstr,sizeof(tmpstr),0))>0){
            printf("[%s]%s\n",name,tmpstr);
        }
        printf("finished rap client[%d]\n",skt);
        shutdown(skt,SHUT_RDWR);
    }
}

void handle_echo(int skt){
    if(skt<=0){
        printf("accept client failed!\n");
        return ;
    }

    if(fork()==0){
        char name[64];
        memset(name,0,sizeof(name));
        recv(skt,name,sizeof(name),0);
        printf("accept client %s\n",name);

        const int tmplen = 128;
        char itmp[tmplen];
        char otmp[tmplen];

        char tagstr[] = "s2c:";

        sprintf(otmp,"hey,welcome to tinycgi, %s!",name);
        int rt = send(skt,otmp,tmplen,0);

        while((rt=recv(skt,itmp,tmplen,0))>0){
            printf("c2s[%s]: %s\n",name,itmp);
            memcpy(otmp,tagstr,strlen(tagstr));
            strcpy(otmp+strlen(tagstr),itmp);
            send(skt,otmp,tmplen,0);
        }
        shutdown(skt,SHUT_RDWR);
    }
}

void handle_http(int skt){
    if(skt<=0){
        printf("accept client failed!\n");
        return ;
    }

    if(fork()==0){
        const int tmplen = 4096;
        char itmp[tmplen];
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
        recv(skt,itmp,tmplen,0);
        printf("\n\nreceive:\n%s\n",itmp);
        sprintf(otmp,
                "HTTP/1.1 200 OK\r\n"
                "Server: TinyCGI/0.0.1\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %u\r\n\r\n"
                "%s",msglen,msg);
        send(skt,otmp,tmplen,0);
    }
}


