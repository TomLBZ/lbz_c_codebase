#include "networking.h"
#include <stdio.h>
#include <stdlib.h>

NetState startNetworking(NetConfig* config){
    switch (config->role)
    {
    case SERVER:
        return startServer(config);
    case CLIENT:
        return startClient(config);
    default:
        return UNKNOWN_ROLE;
    }
}

NetState startServer(NetConfig* config){
    switch (config->protocol)
    {
    case TCP:
        return startTCPServer(config);
    case UDP:
        return startUDPServer(config);
    default:
        return UNKNOWN_PROTOCOL;
    }
}

NetState startClient(NetConfig* config){
    switch (config->protocol)
    {
    case TCP:
        return startTCPClient(config);
    case UDP:
        return startUDPClient(config);
    default:
        return UNKNOWN_PROTOCOL;
    }
}

NetState initHost(NetConfig* config, int* outsock){
    // check host name
    if(!config->ip) return BAD_CONFIG;
    Hostent* sh;
    if ((sh=gethostbyname(config->ip))==NULL) return HOSTNAME_ERROR;
    config->host = sh;
    int sock = socket(
        config->ipType == IPv6 ? AF_INET6 : AF_INET, 
        config->protocol == TCP ? SOCK_STREAM : SOCK_DGRAM, 
        config->protocol == TCP ? IPPROTO_TCP : IPPROTO_UDP
    );
    if(sock<0) return SOCKET_ERROR;
    *outsock = sock;
    return STATE_SUCCESS;
}

NetState startTCPServer(NetConfig* config){
    printf("TCP Server Starting...\n");
    int sock = 0;
    NetState hostState = initHost(config, &sock);
    if(hostState != STATE_SUCCESS) return hostState;
    // setup socket
    int fd, client_len; // file descriptor, client addr size
    Sockaddr_in server, client; // useful addresses
    //Servent* service;   // struct servent service
    server.sin_family = config->ipType == IPv6 ? AF_INET6 : AF_INET;    // ip
    server.sin_addr.s_addr = htonl(INADDR_ANY); // big endian any address
    server.sin_port = htons((uint16_t)config->port); // port
    // bing port and listen
    int ret = bind(sock, (Sockaddr*)&server, sizeof(server));
    if(ret < 0) return BINDING_ERROR;
    ret = listen(sock, config->queueSize);    // setup queue for pending connections
    if(ret < 0) return LISTENING_ERROR;
    // service loop
    config->isRunning = true;
    NetState ns = STATE_SUCCESS;
    while(config->isRunning){
        // wait for connection
        printf("Waiting for Connection.\n");
        client_len = sizeof(client);    // size of addr
        fd = accept(sock, (Sockaddr*)&client, (socklen_t*)&client_len); //blocking accept
        if(fd<0) return ACCEPTING_ERROR;
        printf("Connected to Client.\n");
        // run services
        for (size_t i = 0; i < config->ntcpfuncs; i++)
        {
            ns = (*(config->tcpfuncs[i]))(fd, config->bufferSize);
            if(ns != STATE_SUCCESS) {
                config->isRunning = false;
                break;
            }
        }
        close(fd);  //terminate descriptor
    }
    close(sock);    // close socket
    return ns;
}

NetState startTCPClient(NetConfig* config){
    printf("TCP Client Starting...\n");
    int sock = 0;
    NetState hostState = initHost(config, &sock);
    if(hostState != STATE_SUCCESS) return hostState;
    // connect to server
    Sockaddr_in server;
    server.sin_family = config->ipType == IPv6 ? AF_INET6 : AF_INET;    // ip
	server.sin_port = htons((uint16_t)config->port);
    In_addr** addrs = (In_addr**)config->host->h_addr_list;
	memcpy(&(server.sin_addr.s_addr), *addrs, sizeof(In_addr));
	bzero(&(server.sin_zero), 8);
	int ret = connect(sock, (Sockaddr*)&server, sizeof(server));  //connect the socket with the host
    if(ret < 0) return CONNECTION_ERROR;
    // functionalities
    printf("Connected to Server.\n");
    NetState ns = STATE_SUCCESS;
    for (size_t i = 0; i < config->ntcpfuncs; i++)
    {
        ns = (*(config->tcpfuncs[i]))(sock, config->bufferSize);
        if(ns != STATE_SUCCESS) {
            config->isRunning = false;
            break;
        }
    }
    return ns;
}

NetState startUDPServer(NetConfig* config){
    printf("UDP Server Starting...\n");
    int sock = 0;
    NetState hostState = initHost(config, &sock);
    if(hostState != STATE_SUCCESS) return hostState;
    Sockaddr_in server, client; // useful addresses
    server.sin_family = config->ipType == IPv6 ? AF_INET6 : AF_INET;    // ip
    server.sin_addr.s_addr = htonl(INADDR_ANY); // big endian any address
    server.sin_port = htons((uint16_t)config->port); // port
	bzero(&(server.sin_zero), 8);
    memset(&client, 0, sizeof(client));
    // bing port
    int ret = bind(sock, (Sockaddr*)&server, sizeof(server));
    if(ret < 0) return BINDING_ERROR;
    // service loop
    config->isRunning = true;
    NetState ns = STATE_SUCCESS;
    socklen_t len = sizeof(Sockaddr_in);
    while(config->isRunning){
        // run services
        for (size_t i = 0; i < config->nudpfuncs; i++)
        {
            ns = (*(config->udpfuncs[i]))(sock, config->bufferSize, &client, &len);
            if(ns != STATE_SUCCESS) {
                config->isRunning = false;
                break;
            }
        }
    }
    close(sock);    // close socket
    return ns;
}

NetState startUDPClient(NetConfig* config){
    printf("UDP Client Starting...\n");
    int sock = 0;
    NetState hostState = initHost(config, &sock);
    if(hostState != STATE_SUCCESS) return hostState;
    // prepare server addr
    Sockaddr_in server;
    server.sin_family = config->ipType== IPv6 ? AF_INET6 : AF_INET;
    server.sin_port = htons((uint16_t)config->port);
    In_addr** addrs = (In_addr**)config->host->h_addr_list;
	memcpy(&(server.sin_addr.s_addr), *addrs, sizeof(In_addr));
	bzero(&(server.sin_zero), 8);
    // functionalities
    printf("Started UDP Functionalities.\n");
    NetState ns = STATE_SUCCESS;
    socklen_t len = sizeof(Sockaddr_in);
    for (size_t i = 0; i < config->nudpfuncs; i++)
    {
        ns = (*(config->udpfuncs[i]))(sock, config->bufferSize, &server, &len);
        if(ns != STATE_SUCCESS) {
            config->isRunning = false;
            break;
        }
    }
    close(sock);
    return ns;
}

void addTCPService(tcpfptr funcptr, NetConfig* config){
    if(!funcptr || !config) return;
    if(config->protocol != TCP) return;
    if(!config->ntcpfuncs){
        config->tcpfuncs = (tcpfptr*)malloc(sizeof(tcpfptr));
    } else {
        config->tcpfuncs = (tcpfptr*)realloc(config->tcpfuncs, sizeof(tcpfptr)*(config->ntcpfuncs+1));
    }
    config->tcpfuncs[config->ntcpfuncs] = funcptr;
    config->ntcpfuncs++;
}

void addUDPService(udpfptr funcptr, NetConfig* config){
    if(!funcptr || !config) return;
    if(config->protocol != UDP) return;
    if(!config->nudpfuncs){
        config->udpfuncs = (udpfptr*)malloc(sizeof(udpfptr));
    } else {
        config->udpfuncs = (udpfptr*)realloc(config->udpfuncs, sizeof(udpfptr)*(config->nudpfuncs+1));
    }
    config->udpfuncs[config->nudpfuncs] = funcptr;
    config->nudpfuncs++;
}