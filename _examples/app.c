#include "headers/app.h"
#include "headers/services.h"
/*variable for services*/

#define HK_SER      "37.123.196.139"
#define LOS_SER     "216.24.181.167"
#define SG_SER      "45.76.188.170"
#define LOCALHOST   "127.0.0.1"

void testNetwork();

int main(){
    testNetwork();
    return 0;
}

void testNetwork(){
    bool isTCP = false;
    bool isServer = false;
    bool isIPv6 = false;
    NetConfig config = {
        .ip         =   strdup(LOCALHOST), 
        .ipType     =   isIPv6 ? IPv6 : IPv4,
        .port       =   5350, 
        .protocol   =   isTCP ? TCP : UDP, 
        .role       =   isServer ? SERVER : CLIENT,
        .queueSize  =   5,
        .bufferSize =   1024
    };
    initServicesVariables(10, 1);
    if (isTCP) addTCPService(isServer ? testTCPserviceS : testTCPserviceC, &config);
    else addUDPService(isServer ? demoUDPserviceS : demoUDPserviceC, &config);
    NetState ns = startNetworking(&config);
    if(ns != STATE_SUCCESS) printf("Error code: %d\n", (int)ns);
    else printf("Task Succeeded!\n");
}