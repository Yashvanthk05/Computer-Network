#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>

int main(int argc,char *argv[]){
	int sockfd,newsockfd,port,n,clilen,expected_frame=0;
	char buffer[256];
	struct sockaddr_in serv_addr,cli_addr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		perror("Error on Opening the Socket");
		exit(1);
	}
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
	while(1){
		bzero(buffer,256);
		n=read(newsockfd,buffer,255);
		if(n<0){
			perror("Error on Reading from Client");
			exit(1);
		}
		if(strlen(buffer)==0){
			continue;
		}
		int received_frame=buffer[0]-'0';
		printf("Received Frame: %d\n",received_frame);
		if(received_frame==expected_frame){
			printf("Frame %d is Correct. Sending ACK : %d\n",received_frame,!received_frame);
			sprintf(buffer,"%d",!received_frame);
			n=write(newsockfd,buffer,strlen(buffer));
			if(n<0){
				perror("Error Writing to the Client");
				exit(1);
			}
			expected_frame=!expected_frame;
		}else{
			printf("Frame %d is Incorrect. Expected Frame: %d\n",received_frame,expected_frame);
			sprintf(buffer,"%d",!expected_frame);
			n=write(newsockfd,buffer,sizeof(buffer));
			if(n<0){
				perror("Error on Writing");
				exit(1);
			}
		}
	}
	return 0;
}
