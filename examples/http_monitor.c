// http_monitor.c
// Sample monitor that only allows HTTP traffic
// This sample will only check request method and "HTTP" header in response

#include "monitor_helper.h"
#include "pktlab.h"
#include "string.h"

const char* http_methods[] = {"GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "TRACE", "CONNECT"};


int32_t check_pktlab_message_nopen(uint8_t sktid, uint8_t family, uint8_t proto,
                                   uint32_t rbufsz, uint8_t laddrlen,
                                   uint8_t lportlen, uint8_t raddrlen,
                                   uint8_t rportlen, const void* laddrptr,
                                   const void* lportptr, const void* raddrptr,
                                   const void* rportptr,
                                   void* persistent_memory,
                                   uint32_t persistent_memory_len) {
    if (proto == PKTLAB_TCP_PROTO) {
        return 0;
    }
    // prevent proto to be 0
    return 1 + proto;
}

int32_t check_pktlab_message_nclose(uint8_t sktid, void* persistent_memory,
                                    uint32_t persistent_memory_len) {
    return 0;
}

int32_t check_pktlab_message_nsend(uint8_t sktid, uint8_t family, uint8_t proto,
                                   pktlab_time_t time, uint16_t tag,
                                   uint8_t raddrlen, const void* raddrptr,
                                   uint8_t rportlen, const void* rportptr,
                                   uint32_t len, void* persistent_memory,
                                   uint32_t persistent_memory_len) {
    if (proto == PKTLAB_TCP_PROTO) {
        if (len < 7) {
            return -1;
        }
        char header[3];
        // only check the first 3 bytes is enough
        int32_t res = read_vmem_region(PKTLAB_VMEMADDR_CURPKT_DATA, &header, 3);
        if (res != 0) {
            return -1;
        }

        for (int i = 0; i < 8; i++) {
            if (memcmp(header, http_methods[i], 3) == 0) {
                return 0;
            }
        }
        return INT16_MAX;
    }
    return 1 + proto;
}

int32_t check_pktlab_message_ndata(uint8_t sktid, uint8_t family, uint8_t proto,
                                   pktlab_time_t time, uint8_t raddrlen,
                                   const void* raddrptr, uint8_t rportlen,
                                   const void* rportptr, uint32_t len,
                                   void* persistent_memory,
                                   uint32_t persistent_memory_len) {
    if (proto != PKTLAB_TCP_PROTO) {
        return 1 + proto;
    }
    // check header is HTTP
    char header[4];
    int32_t res = read_vmem_region(PKTLAB_VMEMADDR_CURPKT_DATA, &header, 4);
    if (res != 0) {
        return -1;
    }
    const char* http = "HTTP";
    if (memcmp(header, http, 4) == 0) {
        return 0;
    } else {
        return 1;
    }
}

int32_t check_pktlab_message_ncap(uint8_t sktid, uint8_t family, uint8_t proto,
                                  pktlab_time_t endtime,
                                  void* persistent_memory,
                                  uint32_t persistent_memory_len) {
    // only allow TCP
    // should not be called
    return 1 + proto;
}
