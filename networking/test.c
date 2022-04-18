#include "networking.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void testStr();
void testUI();
void testNetwork();
NetState testTCPserviceS(int fdin, int fdout, size_t buffsize);
NetState testTCPserviceC(int fdin, int fdout, size_t buffsize);
NetState testUDPserviceS(int fdin, int fdout, size_t buffsize, Sockaddr* addr, socklen_t addrLen, socklen_t* len);
NetState testUDPserviceC(int fdin, int fdout, size_t buffsize, Sockaddr* addr, socklen_t addrLen, socklen_t* len);

int main(){
    //testStr();
    testNetwork();
    return 0;
}

void testNetwork(){
    bool isServer = false;
    bool isTCP = true;
    bool isIPv6 = false;
    NetConfig config = {
        .ip         =   strdup("127.0.0.1"), 
        .ipType     =   isIPv6 ? IPv6 : IPv4,
        .port       =   5350, 
        .protocol   =   isTCP ? TCP : UDP, 
        .role       =   isServer ? SERVER : CLIENT,
        .queueSize  =   5,
        .bufferSize =   1024
    };
    if (isTCP) addTCPService(isServer ? testTCPserviceS : testTCPserviceC, &config);
    else addUDPService(isServer ? testUDPserviceS : testUDPserviceC, &config);
    NetState ns = startNetworking(&config);
    if(ns != STATE_SUCCESS) printf("Error code: %d\n", (int)ns);
}

NetState testTCPserviceS(int fdin, int fdout, size_t buffsize){
    unsigned char buf[buffsize];
    int count;
    while((count=read(fdin, buf, 1024)) > 0){
        unsigned char* p;
        for ( p = buf; p < buf+count; p++)
        {
            if(islower(*p)){
                *p+=13;
                if(*p>'z')*p-=26;
            }
        }
        write(fdout, buf, count);
    }
    return STATE_SUCCESS;
}

NetState testTCPserviceC(int fdin, int fdout, size_t buffsize){
    unsigned char buf[buffsize];
    int count;
    while((count=read(0, buf, buffsize))>0){
        write(fdout, buf, count);   // communicate with server
        read(fdin, buf, count); // receive from server
        write(1, buf, count);   // write to stdout
    }
    return STATE_SUCCESS;
}

NetState testUDPserviceS(int fdin, int fdout, size_t buffsize, Sockaddr* addr, socklen_t addrLen, socklen_t* len){
    char buf[buffsize];
    int n = recvfrom(fdin, &buf, buffsize, 0, addr, len);
    if(n==-1) return RECEIVING_ERROR;
    buf[n] = '\0';
    printf("Received: %s\n", buf);
    return STATE_SUCCESS;
}

NetState testUDPserviceC(int fdin, int fdout, size_t buffsize, Sockaddr* addr, socklen_t addrLen, socklen_t* len){
    char buf[buffsize];
    printf("Please input a string (less than 50 characters):\n");
    while (fgets(buf, buffsize, stdin) == NULL) {
		printf("error input\n");
	}
    sendto(fdout, &buf, strlen(buf), 0, addr, addrLen);
    printf("Sent.\n");
    return STATE_SUCCESS;
}