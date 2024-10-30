#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>

int main(int argc,char *argv[]){
    int sockfd,port,n;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    if(argc<3){
        fprintf(stderr,"Usage %s Hostname\n",argv[0]);
        exit(0);
    }
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    port=atoi(argv[2]);
    if(sockfd<0){
        perror("Error on Creating the Socket");
        exit(1);
    }
    server = gethostbyname(argv[1]);
    if(server==NULL){
        fprintf(stderr,"No such Host");
        exit(0);
    }
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    bcopy((char*)server->h_addr_list[0],(char*)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port=htons(port);
    socklen_t servlen=sizeof(serv_addr);
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n=sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    if(n<0){
        perror("Error on Sending to Server");
        exit(1);
    }
    n=recvfrom(sockfd,buffer,255,0,(struct sockaddr*)&serv_addr,&servlen);
    if(n<0){
        perror("Error on Receiving from the Server");
        exit(1);
    }
    printf("%s\n",buffer);
    return 0;
}