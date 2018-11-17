#include"lib/unpv13e/lib/unp.h"
#include<stdio.h>

void dg_echo(int sockfd,SA *sockaddr,socklen_t len);
int is_inner(char * addr);
int parseDnsData(void * data,size_t len,int sockfd,SA * sock_addr,socklen_t sock_len,int flag);
void getserver_addr(int flag,struct sockaddr_in * parse_server_addr);


static const char * PUBLIC_DNS = "8.8.8.8";
static const char * PRIVATE_DNS = "114.114.114.114";


int main(int argc,char **argv){
    int sockfd;
    int ret = 0;
    struct sockaddr_in serveraddr, cliaddr;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(53);
    ret = bind(sockfd, (SA *)&serveraddr, sizeof(serveraddr));
    printf("bind result: %d",ret);
    dg_echo(sockfd,(SA *) &cliaddr,sizeof(cliaddr));
}  

void dg_echo(int sockfd,SA *sockaddr,socklen_t client_len){
    int n;
    socklen_t len;
    char mesg[512];
    char url[256];
    for(; ; ){
        len = client_len;
        printf("start Revc mesg\n");        
        n = recvfrom(sockfd,mesg,512,0,sockaddr,&len);
        printf("Revc mesg %s\n",&mesg[13]);
        sprintf(url,"%s",&mesg[13]);
        if(is_inner(url) == 0){
            parseDnsData(mesg,n,sockfd,sockaddr,client_len,1);
        }
        else{
             parseDnsData(mesg,n,sockfd,sockaddr,client_len,0);
    
        }
        ;
    }
}

int is_inner(char * addr){
    printf("get url %s\n",addr);
    if(strstr(addr,"h3c")!= NULL  || strstr(addr,"baidu")!= NULL){
        printf("is an inner address");
        
         return 0;
    }
    else{
        printf("is not an inner address");
         return -1;
    }
   
}

int parseDnsData(void * data,size_t len,int server_sock_fd,SA *client_sock_addr,socklen_t sock_len,int flag){
    printf("start parse\n");
    int parse_sockfd =0;
    int n = 0;
    char recvline[512];
    struct sockaddr_in parse_server_addr;
    parse_sockfd =  socket(AF_INET,SOCK_DGRAM,0);
    bzero(&parse_server_addr, sizeof(parse_server_addr));
    parse_server_addr.sin_family = AF_INET;
    
    //设置请求dns服务器的地址
    getserver_addr(flag,&parse_server_addr);


    parse_server_addr.sin_port = htons(53);
    sendto(parse_sockfd,data,len,0,(struct sockaddr *)&parse_server_addr,sizeof(parse_server_addr));
    n =  recvfrom(parse_sockfd,recvline,512,0,NULL,NULL);
    printf("recv data size %d\n",n);
    printf("send to client result\n");
    sendto(server_sock_fd,recvline,512,0,client_sock_addr,sock_len);
    return 0;
}

void getserver_addr(int flag,struct sockaddr_in *parse_server_addr){

    if(flag == 0){
        printf("parse by public dns %s\n",PUBLIC_DNS);
        inet_pton(AF_INET,PUBLIC_DNS,&parse_server_addr->sin_addr);
    }
    else{
        printf("parse by private dns %s\n",PRIVATE_DNS);
        inet_pton(AF_INET,PRIVATE_DNS,&parse_server_addr->sin_addr);
    }
}
