#include "../headers/lbzstr.h"
#include "../headers/lbzcurses.h"
#include "../headers/lbznetworking.h"
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

void testUI(){
    UIVars ui = lbzCursesUI();
    Control* frame = createControl(0,0,ui.w,ui.h,1,COLOR_PAIR(C_BLACK_CYAN));
    Textbox* frametitle = createTextbox(20,0,20,2,COLOR_PAIR(C_BLUE_BLACK),MIDDLE,NORMAL);
    char frametitletext[] = "Helloooooooo\nWorld!";
    frametitle->text = frametitletext;
    addTextbox(frame, frametitle);
    addControl(&ui, frame);
    //debug();
    updateUI(&ui);
    getchar();
    destroyUI(&ui);
}

void testStr(){
    char str[] = "Hello \nWorld!";
    char str2[] = "888 999 777 666";
    char* str2_cleaned = strReplace(str2, " ", "");
    size_t npages = 0;
    size_t width = 20;
    size_t height = 2;
    char*** pages = strToPages(str, width, height, &npages);
    char buffer[SPRTF_BUFF_SIZE];
    char* p = buffer;
    int o1,o2;
    o1 = sprintpgs(p, pages, npages, height);
    p += o1;
    o2 = sprintlnsep(p, str2_cleaned, -1);
    p += o2;
    freePages(pages, npages, height);
    sprintf(p, "Memory Freed!\n");
    printf("%s",buffer);
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
        .queueSize  =   NET_QUEUESIZE,
        .bufferSize =   NET_BUFFERSIZE
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