#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.1.7"
#define SERVER_PORT 1987

void rap_client(const char *name,const sockaddr &server_addr){
    if(!name || strlen(name)<=0) return ;

    int skt = socket(AF_INET,SOCK_STREAM,0);
    if(skt<=0) return ;

    switch(fork()){
        case 0:
            if(connect(skt,&server_addr,sizeof(server_addr))){
                printf("connect to server failed!");
                return ;
            }
            send(skt,name,strlen(name),MSG_DONTWAIT);
            char tmpstr[128];
            for(int i=0;;i++){
                sprintf(tmpstr,"rap message %d, check it",i);
                send(skt,tmpstr,strlen(tmpstr),MSG_DONTWAIT);
                printf("%s\n",tmpstr);
                usleep(100000);
            }
            break;
        case -1:
            printf("fork error!");
            return ;
        default:
            //parent here
            break;
    }
}

void echo_client(const char *name,const sockaddr &server_addr){
    if(!name || strlen(name)<=0) return ;

    int skt = socket(AF_INET,SOCK_STREAM,0);
    if(skt<=0) return ;

    if(connect(skt,&server_addr,sizeof(server_addr))){
        printf("connect to server failed!\n");
        return ;
    }

    const int tmplen = 4096; 
    char itmp[tmplen];
    char otmp[tmplen];

    if(send(skt,name,strlen(name),0)<=0){
        printf("send to server failed!\n");
        return ;
    }

    while(strcmp("exit",otmp)){
        if(recv(skt,itmp,tmplen,0)<=0){
            printf("server disconnected!\n");
            break;
        }
        printf("%s\n",itmp);
        printf("input: ");
        scanf("%s",otmp);
        if(strlen(otmp)>0){
            send(skt,otmp,strlen(otmp),0);
        }
    }
    shutdown(skt,SHUT_RDWR);
}

typedef void (*CLIENT)(const char*,const sockaddr &);

int main(int argc,char **argv){
    sockaddr_in server_addr = {
        AF_INET,
        htons(SERVER_PORT),
        inet_addr(SERVER_IP),
        0};
    CLIENT client = echo_client;
    if(0==strcasecmp("rap",argv[1])){
        client = rap_client;
    }else if(0==strcasecmp("echo",argv[1])){
        client = echo_client;
    }
    client(argv[2],(sockaddr &)server_addr);
    /*
    char name[32];
    for(int i=1;i<=100;++i){
        sprintf(name,"%d-client",i);
        client(name,(sockaddr &)server_addr);
    }*/
    return 0;
}


