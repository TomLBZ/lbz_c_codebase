#include "services.h"
// file scope variable
UDP_Stats s_udp_stats;

UDP_Stats* initServicesVariables(int pktsize, int batchsize){
    s_udp_stats.batchsize = batchsize;
    s_udp_stats.packetsize = pktsize;
    s_udp_stats._batchcount = 0;
    s_udp_stats._datasize = 0;
    s_udp_stats._packetcount = 0;
    s_udp_stats._rate = 0;
    s_udp_stats._running = false;
    s_udp_stats._time = 0;
    return &s_udp_stats;
}

NetState testTCPserviceS(int fd, size_t buffsize){
    unsigned char buf[buffsize];
    int count;
    while((count=read(fd, buf, 1024)) > 0){
        unsigned char* p;
        for ( p = buf; p < buf+count; p++)
        {
            if(islower(*p)){
                *p+=13;
                if(*p>'z')*p-=26;
            }
        }
        write(fd, buf, count);
    }
    return STATE_SUCCESS;
}

NetState testTCPserviceC(int fd, size_t buffsize){
    unsigned char buf[buffsize];
    int count;
    while((count=read(0, buf, buffsize))>0){
        write(fd, buf, count);   // communicate with server
        read(fd, buf, count); // receive from server
        write(1, buf, count);   // write to stdout
    }
    return STATE_SUCCESS;
}

NetState testUDPserviceS(int sock, size_t buffsize, Sockaddr_in* addr, socklen_t* len){
    char buf[buffsize];
    int n = recvfrom(sock, &buf, buffsize, 0, (Sockaddr*)addr, len);
    if(n==-1) return RECEIVING_ERROR;
    buf[n] = '\0';
    printf("Received: %s\n", buf);
    return STATE_SUCCESS;
}

NetState testUDPserviceC(int sock, size_t buffsize, Sockaddr_in* addr, socklen_t* len){
    char buf[buffsize];
    printf("Please input a string (less than 50 characters):\n");
    while (fgets(buf, buffsize, stdin) == NULL) {
		printf("error input\n");
	}
    sendto(sock, &buf, strlen(buf), 0, (Sockaddr*)addr, sizeof(*addr));
    printf("Sent.\n");
    return STATE_SUCCESS;
}

NetState demoUDPserviceS(int sock, size_t buffsize, Sockaddr_in* addr, socklen_t* len){
    printf("Receiving Data...\n");
    bool end = false;
    int n = 0;
    FILE* fp;
    ACK ack = {.len = 0, .num = 1};
    Messageframe mf;
    // init total length
    while (mf.flag != DF_INIT) if ((n= recvfrom(sock, &mf, sizeof(Messageframe), 0, (Sockaddr*)addr, len))==-1) return RECEIVING_ERROR;
    uint32_t lsize = mf.num;
    uint32_t nbytes = 0;
	char* buf = (char *) malloc (lsize);
	if (buf == NULL) return MEMORY_ERROR;
	if ((n=sendto(sock, &ack, sizeof(ack), 0, (Sockaddr*)addr, *len))==-1) return SENDING_ERROR;
    // init total packet count
    while (mf.flag != DF_PKTLEN) if ((n= recvfrom(sock, &mf, sizeof(Messageframe), 0, (Sockaddr*)addr, len))==-1) return RECEIVING_ERROR;
    uint32_t lpktsize = mf.num;
    uint32_t lpktcount = lsize / lpktsize;
    uint32_t npkts = 0;
    if (lpktcount * lpktsize < lsize) lpktcount++;
	if ((n=sendto(sock, &ack, sizeof(ack), 0, (Sockaddr*)addr, *len))==-1) return SENDING_ERROR;
    // init batch size
    while (mf.flag != DF_BATCHSIZE) if ((n= recvfrom(sock, &mf, sizeof(Messageframe), 0, (Sockaddr*)addr, len))==-1) return RECEIVING_ERROR;
    uint32_t lbsize = mf.num;
    uint32_t lbcount = lpktcount / lbsize;
    uint32_t batchSize = lbsize;
    uint32_t nbatches = 0;
    if(lbcount * lbsize < lpktcount) lbcount++;
	if ((n=sendto(sock, &ack, sizeof(ack), 0, (Sockaddr*)addr, *len))==-1) return SENDING_ERROR;
    // allocating the dynamic dataframe memory
    size_t dfsize = lpktsize * sizeof(char) + DF_OFFSET;
    Dataframe* dfptr = malloc(dfsize);
    // main loop
    printf("Preparing for %u packets of %u in %u batches of %u, %u bytes in total.\n", lpktcount, lpktsize, lbcount, lbsize, lsize);
    while (!end)
    {
		if ((n= recvfrom(sock, dfptr, dfsize, 0, (Sockaddr*)addr, len))==-1) return RECEIVING_ERROR;
        batchSize--;
        npkts++;
        if(batchSize == 0){
            if ((n=sendto(sock, &ack, sizeof(ack), 0, (Sockaddr*)addr, *len))==-1) return SENDING_ERROR;
            batchSize = lbsize;
            nbatches++;
        }
        if (dfptr->flag == DF_STOP || nbytes == lsize || npkts == lpktcount){//if client asks to stop
			end = 1;
			n --;
            dfptr->len--;
		}
		memcpy((buf+nbytes), dfptr->data, dfptr->len);
        nbytes += dfptr->len;
    }
	if ((n=sendto(sock, &ack, sizeof(ack), 0, (Sockaddr*)addr, *len))==-1) return SENDING_ERROR;
    printf("Received %u packets in %u batches, %u bytes in total.\n", npkts, nbatches, nbytes);
	if ((fp = fopen (SERVER_FILE,SERVER_FILEOPTION)) == NULL) return IO_ERROR;
    fwrite(buf, 1, nbytes, fp);
    fclose(fp);
	printf("File written: %d bytes\n", (int)nbytes);
    free(buf);
    return STATE_SUCCESS;
}

NetState demoUDPserviceC(int sock, size_t buffsize, Sockaddr_in* addr, socklen_t* len){
    char *buf;
    printf("Opening file...\n");
    // prepare file info
    FILE* fp = fopen(CLIENT_FILE, CLIENT_FILEOPTION);
    if(!fp) return IO_ERROR;
    fseek(fp, 0, SEEK_END);
    s_udp_stats._datasize = ftell(fp);
    rewind(fp);
    // mem allocation and copy the file
	buf = (char *) malloc (s_udp_stats._datasize);
	if (buf == NULL) return MEMORY_ERROR;
    fread(buf, 1, s_udp_stats._datasize, fp);
    buf[s_udp_stats._datasize] = '\0';
    // prepare for main loop
    s_udp_stats._running = true;
	printf("pcktsize,time1,time2,time4,rate1,rate2,rate4\n");
    for (; s_udp_stats.packetsize <= _UDP_MAX_PKTSZ; s_udp_stats.packetsize+=10)
    {
        float times[3], rates[3];
        for (int i = 0; i <= 2; i++)
        {
            s_udp_stats.batchsize = (uint32_t)pow(2, i);
            NetState tmpns = demoUDPserviceC_Inner(buf, sock, addr, len);
            if (tmpns != STATE_SUCCESS) return tmpns;
            times[i] = s_udp_stats._time;
            rates[i] = s_udp_stats._rate;
        }
        printf("%d,%f,%f,%f,%f,%f,%f\n",s_udp_stats.packetsize,times[0],times[1],times[2],rates[0],rates[1],rates[2]);
    }
    free(buf);
    return STATE_SUCCESS;
}

NetState demoUDPserviceC_Inner(char* buf, int sock, Sockaddr_in* addr, socklen_t* len){
    // populate s_udp_stats
    s_udp_stats._packetcount = s_udp_stats._datasize / s_udp_stats.packetsize; // number of packets
    if (s_udp_stats._packetcount * s_udp_stats.packetsize < s_udp_stats._datasize) s_udp_stats._packetcount++;
    if (s_udp_stats._packetcount == 0) s_udp_stats._packetcount++; 
    s_udp_stats._batchcount = s_udp_stats._packetcount / s_udp_stats.batchsize;// number of batches
    if(s_udp_stats._batchcount * s_udp_stats.batchsize < s_udp_stats._packetcount) s_udp_stats._batchcount++;
    if (s_udp_stats._batchcount == 0) s_udp_stats._batchcount++;
    // send requests to the server to notify variables
    Messageframe mf;
    ssize_t n = 0; // return value for socket apis
    ACK ack; // data structure sent back from server
    mf.flag = DF_INIT; // init data size
    mf.num = s_udp_stats._datasize;
    if((n = sendto(sock, &mf, sizeof(Messageframe), 0, (Sockaddr*)addr, sizeof(*addr))) == -1) return SENDING_ERROR;
    if((n = recvfrom(sock, &ack, sizeof(ack), 0, (Sockaddr*)addr, len)) == -1) return RECEIVING_ERROR;
    mf.flag = DF_PKTLEN; // init packet len
    mf.num = s_udp_stats.packetsize;
    if((n = sendto(sock, &mf, sizeof(Messageframe), 0, (Sockaddr*)addr, sizeof(*addr))) == -1) return SENDING_ERROR;
    if((n = recvfrom(sock, &ack, sizeof(ack), 0, (Sockaddr*)addr, len)) == -1) return RECEIVING_ERROR;
    mf.flag = DF_BATCHSIZE; // init batch size
    mf.num = s_udp_stats.batchsize;
    if((n = sendto(sock, &mf, sizeof(Messageframe), 0, (Sockaddr*)addr, sizeof(*addr))) == -1) return SENDING_ERROR;
    if((n = recvfrom(sock, &ack, sizeof(ack), 0, (Sockaddr*)addr, len)) == -1) return RECEIVING_ERROR;
    // allocating the dynamic dataframe memory
    size_t dfsize = s_udp_stats.packetsize * sizeof(char) + DF_OFFSET;
    Dataframe* dfptr = malloc(dfsize);
    long bufoffset = 0;
    long seglen = 0;
    uint32_t pktsleftinbatch = s_udp_stats.batchsize;
    // getting current time
    Timeval sendt, recvt;
    gettimeofday(&sendt, NULL);
    while(bufoffset <= s_udp_stats._datasize)
    {
        if ((s_udp_stats._datasize+1-bufoffset) <= s_udp_stats.packetsize) {
            seglen = s_udp_stats._datasize+1-bufoffset;
            dfptr->flag = DF_STOP;
        } else seglen = s_udp_stats.packetsize;
        dfptr->len = seglen;
        memcpy(dfptr->data, (buf+bufoffset), seglen);
        if((n = sendto(sock, dfptr, dfsize, 0, (Sockaddr*)addr, sizeof(*addr))) == -1) return SENDING_ERROR;
        bufoffset += seglen;
        pktsleftinbatch--;
        dfptr->id++;
        if(pktsleftinbatch == 0){
            // wait for ACK after completing a batch
            if ((n = recvfrom(sock, &ack, sizeof(ack), 0, (Sockaddr*)addr, len)) == -1) return RECEIVING_ERROR;
            pktsleftinbatch = s_udp_stats.batchsize;
        }
    }
    if ((n = recvfrom(sock, &ack, sizeof(ack), 0, (Sockaddr*)addr, len)) == -1) return RECEIVING_ERROR;
    if(ack.num != 1 || ack.len != 0) return TRANSMISSION_ERROR;
    // get current time
    gettimeofday(&recvt, NULL);
    if((recvt.tv_usec -= sendt.tv_usec) < 0){
        --recvt.tv_sec;
        recvt.tv_usec += 1000000;
    }
    recvt.tv_sec -= sendt.tv_sec;
    // total time
    s_udp_stats._time = recvt.tv_sec * 1000.0 + recvt.tv_usec / 1000.0;
    s_udp_stats._rate = s_udp_stats._datasize / s_udp_stats._time;
    return STATE_SUCCESS;
}
