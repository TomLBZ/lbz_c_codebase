#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#if !defined(NET_MACROS)
#define NET_MACROS
    
typedef enum Protocol_t{
    TCP,
    UDP
} Protocol;

typedef enum NetRole_t{
    SERVER,
    CLIENT
} NetRole;

typedef enum NetIPType_t{
    IPv4,
    IPv6
} NetIPType;

typedef enum NetState_t{
    STATE_SUCCESS,
    BAD_CONFIG,
    HOSTNAME_ERROR,
    SOCKET_ERROR,
    BINDING_ERROR,
    LISTENING_ERROR,
    ACCEPTING_ERROR,
    CONNECTION_ERROR,
    SENDING_ERROR,
    RECEIVING_ERROR,
    TRANSMISSION_ERROR,
    UNKNOWN_PROTOCOL,
    UNKNOWN_ROLE,
    SERVICE_ERROR,
    MEMORY_ERROR,
    IO_ERROR
} NetState;

typedef struct hostent Hostent;
typedef struct sockaddr_in Sockaddr_in;
typedef struct sockaddr Sockaddr;
typedef struct servent Servent;
typedef struct in_addr In_addr;
typedef struct addrinfo Addrinfo;
typedef NetState (*tcpfptr)(int fd, size_t buffsize);
typedef NetState (*udpfptr)(int sock, size_t buffsize, Sockaddr_in* addr, socklen_t* len);

typedef struct NetConfig_t{
    char* ip;
    NetIPType ipType;
    size_t port;
    Protocol protocol;
    NetRole role;
    size_t queueSize;
    size_t bufferSize;
    Hostent* host;
    bool isRunning;
    tcpfptr* tcpfuncs;
    int ntcpfuncs;
    udpfptr* udpfuncs;
    int nudpfuncs;
} NetConfig;

/*start networking*/
NetState startNetworking(NetConfig* config);
/*start server*/
NetState startServer(NetConfig* config);
/*start TCP server*/
NetState startTCPServer(NetConfig* config);
/*start UDP server*/
NetState startUDPServer(NetConfig* config);
/*start client*/
NetState startClient(NetConfig* config);
/*start TCP client*/
NetState startTCPClient(NetConfig* config);
/*start UDP client*/
NetState startUDPClient(NetConfig* config);
/*add tcp service*/
void addTCPService(tcpfptr funcptr, NetConfig* config);
/*add udp service*/
void addUDPService(udpfptr funcptr, NetConfig* config);
/*initHost*/
NetState initHost(NetConfig* config, int* outsock);

#endif // NET_MACROS