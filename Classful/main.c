#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void printBinary(int num){
    for(int i=7;i>=0;i--){
        printf("%d",(num>>i)&1);
    }
}

void findClassAndMask(char *ip,unsigned char subnet[4]){
    unsigned char firstOctet;
    sscanf(ip,"%hhu",&firstOctet);
    if(firstOctet<=127){
        printf("Class A\nSubnet Mask: 255.0.0.0\n");
        subnet[0]=255;
    }else if(firstOctet>=128 && firstOctet<=191){
        printf("Class B\nSubnet Mask: 255.255.0.0\n");
        subnet[0]=255,subnet[1]=255;
    }else if(firstOctet>=192 && firstOctet<=223){
        printf("Class C\nSubnet Mask: 255.255.255.0\n");
        subnet[0]=255,subnet[1]=255,subnet[2]=255;
    }else if(firstOctet>=224 && firstOctet<=239){
        printf("Class D(Multicast)\nNo Subnet Mask\n");
        memset(subnet,0,4);
    }else if(firstOctet<=240 && firstOctet<=255){
        printf("Class E(Future Purpose)\nNo Subnet Mask\n");
        memset(subnet,0,4);
    }
}

int main(){
    char ip[16];
    unsigned char bytes[4],subnet[4]={0};
    unsigned char networkid[4],hostid[4];
    printf("Enter the IPV4 Address: ");
    scanf("%15s",ip);
    sscanf(ip,"%hhu.%hhu.%hhu.%hhu",&bytes[0],&bytes[1],&bytes[2],&bytes[3]);
    printf("IPV4 in base 10: %d.%d.%d.%d\n",bytes[0],bytes[1],bytes[2],bytes[3]);
    findClassAndMask(ip,subnet);
    for(int i=0;i<4;i++){
        networkid[i]=bytes[i]&subnet[i];
        hostid[i]=bytes[i]&~subnet[i];
    }
    printf("Network ID: %d.%d.%d.%d\n",networkid[0],networkid[1],networkid[2],networkid[3]);
    printf("Host ID: %d.%d.%d.%d\n",hostid[0],hostid[1],hostid[2],hostid[3]);
    return 0;
}