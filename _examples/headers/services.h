#include "../lbzapi/headers/lbznetworking.h"

#if !defined(SERVICES_MACROS)
#define SERVICES_MACROS
    #define _MINMAX_REASM_BUFSZ 576
    #define _IP_MAX_HEADERSZ    60
    #define _UDP_HEADERSZ       8
    #define _UDP_MAX_PKTSZ      _MINMAX_REASM_BUFSZ - _IP_MAX_HEADERSZ - _UDP_HEADERSZ
    #define CFG_DEFAULT_PKTSZ   _UDP_MAX_PKTSZ
    #define CFG_DEAULT_BCHSZ    1
    #define DF_OFFSET           8
    #define CLIENT_FILE         "myfile.txt"
    #define CLIENT_FILEOPTION   "r+t"
    #define SERVER_FILE         "udpReceive.txt"
    #define SERVER_FILEOPTION   "wt"

typedef struct UDP_Stats_t{
    uint32_t _datasize;
    uint32_t packetsize;
    uint32_t _packetcount;
    uint32_t batchsize;
    uint32_t _batchcount;
    float _time;
    float _rate;
    bool _running;
} UDP_Stats;

typedef enum DataFlag_t{
    DF_NORMAL,
    DF_INIT,
    DF_PKTLEN,
    DF_BATCHSIZE,
    DF_STOP
} DataFlag;

typedef struct ACK_t{
    uint8_t num;
    uint8_t len;
} ACK;

typedef struct Messageframe_t{
    uint16_t flag;
    uint32_t num;
} Messageframe;

typedef struct Dataframe_t{
    uint16_t id;
    uint16_t flag;
    uint32_t len;
    char data[1];
} Dataframe;

typedef struct timeval Timeval;

UDP_Stats* initServicesVariables(int pktsize, int batchsize);

NetState testTCPserviceS(int fd, size_t buffsize);
NetState testTCPserviceC(int fd, size_t buffsize);
NetState testUDPserviceS(int fd, size_t buffsize, Sockaddr_in* addr, socklen_t* len);
NetState testUDPserviceC(int fd, size_t buffsize, Sockaddr_in* addr, socklen_t* len);

NetState demoUDPserviceS(int sock, size_t buffsize, Sockaddr_in* addr, socklen_t* len);
NetState demoUDPserviceC(int sock, size_t buffsize, Sockaddr_in* addr, socklen_t* len);
NetState demoUDPserviceC_Inner(char* buf, int sock, Sockaddr_in* addr, socklen_t* len);

#endif // SERVICES_MACROS