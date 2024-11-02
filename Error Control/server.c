#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>

uint8_t calculateChecksum(const char *data){
    uint32_t sum=0;
    for(int i=0;i<strlen(data);i++){
        sum+=(uint8_t)data[i];
        if(sum>0xFF){
            sum=(sum&0xFF)+1;
        }
    }
    return (uint8_t)~sum;
}

int main(int argc,char *argv[]){
    int sockfd,newsockfd,clilen,port,n;
    uint8_t cal_checksum;
    char buffer[256];
    struct sockaddr_in serv_addr,cli_addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("Error on opening the socket");
        exit(1);
    }
    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    port=5000;
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port=htons(port);
    if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
        perror("Error on Binding");
        exit(1);
    }
    listen(sockfd,5);
    clilen=sizeof(cli_addr);
    newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);
    if(newsockfd<0){
        perror("Error on Accepting");
        exit(1);
    }
    n=read(newsockfd,buffer,255);
    uint8_t cli_checksum;
    if(n<0){
        perror("Error on Receiving Data from Client");
        exit(1);
    }
    cal_checksum=calculateChecksum(buffer);
    n=read(newsockfd,&cli_checksum,sizeof(cli_checksum));
    if(n<0){
        perror("Error on Receiving Checksum from the Client");
        exit(1);
    }
    printf("Received Checksum: %02x\nCalculated Checksum: %02X\n",cli_checksum,cal_checksum);
    bzero(buffer,256);
    if(cal_checksum==cli_checksum){
        n=write(newsockfd,"No Error in the Received Data",30);
    }else{
        n=write(newsockfd,"Received Data is Erroneous",27);
    }
    if(n<0){
        perror("Error Writing to the Client");
        exit(1);
    }
    return 0;
}