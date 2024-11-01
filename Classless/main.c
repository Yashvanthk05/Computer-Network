#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void printBinary(int num){
	for(int i=7;i>=0;i--){
		printf("%d",(num>>i)&1);
	}
}

void findSubnet(int prefix,unsigned char subnet[4]){
	for(int i=0;i<4;i++){
		if(prefix>=8){
			subnet[i]=255;
			prefix-=8;
		}else{
			subnet[i]=(unsigned char)(255<<(8-prefix));
			prefix=0;
		}
	}
}

void findNetwork(unsigned char bytes[4],unsigned char subnet[4],unsigned char network[4]){
	for(int i=0;i<4;i++){
		network[i]=bytes[i]&subnet[i];
	}
}

void findBroadcast(unsigned char network[4],unsigned char broadcast[4],int prefix){
	for(int i=0;i<4;i++){
		if(prefix>=8){
			broadcast[i]=network[i];
			prefix-=8;
		}else{
			broadcast[i]=network[i] | (255>>prefix);
			prefix=0;
		}
	}
}

int main(){
	char ip[20];
	int prefix;
	unsigned char bytes[4],subnet[4]={0};
	unsigned char network[4],broadcast[4];
	printf("Enter the IPV4 Address: ");
	scanf("%19s",ip);
	sscanf(ip,"%hhu.%hhu.%hhu.%hhu/%d",&bytes[0],&bytes[1],&bytes[2],&bytes[3],&prefix);
	printf("IPV4 Address: %d.%d.%d.%d/%d\n",bytes[0],bytes[1],bytes[2],bytes[3],prefix);
	printf("IPV4 Address in Binary: ");
	for(int i=0;i<4;i++){
		printBinary(bytes[i]);
		if(i<3) printf(".");
		else printf("\n");
	}
	printf("Prefix in Binary: ");
	printBinary(prefix);
	printf("\n");
	findSubnet(prefix,subnet);
	findNetwork(bytes,subnet,network);
	findBroadcast(network,broadcast,prefix);
	printf("Network Address: %d.%d.%d.%d\n",network[0],network[1],network[2],network[3]);
	printf("Network Address in Binary: ");
	for(int i=0;i<4;i++){
		printBinary(network[i]);
		if(i<3) printf(".");
		else printf("\n");
	}
	printf("Broadcast Address: %d.%d.%d.%d\n",broadcast[0],broadcast[1],broadcast[2],broadcast[3]);
	printf("Broadcast Address in Binary: ");
	for(int i=0;i<4;i++){
		printBinary(broadcast[i]);
		if(i<3) printf(".");
		else printf("\n");
	}
	printf("Number of Hosts: %d\n",(int)(pow(2,32-prefix)-2));
	return 0;
}
