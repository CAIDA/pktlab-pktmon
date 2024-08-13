// icmp_limit_monitor.c
// Sample monitor that limits the number of ICMP packets sent

#include "monitor_helper.h"
#include "pktlab.h"
#include "string.h"

#define ICMP_LIMIT 10

int32_t check_pktlab_message_nopen(uint8_t sktid, uint8_t family, uint8_t proto,
                                   uint32_t rbufsz, uint8_t laddrlen,
                                   uint8_t lportlen, uint8_t raddrlen,
                                   uint8_t rportlen, const void* laddrptr,
                                   const void* lportptr, const void* raddrptr,
                                   const void* rportptr,
                                   void* persistent_memory,
                                   uint32_t persistent_memory_len) {
    if (proto == PKTLAB_RAW_PROTO) {
        return 0;
    }
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
    if (proto != PKTLAB_RAW_PROTO) {
        return 1 + proto;
    }
    char *str = (char *)persistent_memory + 2;
    uint16_t *count = (uint16_t *)persistent_memory;

    if (persistent_memory_len < 2 + 5) {
        return -1;
    }

    if (memcmp(str, "START", 5) == 0) {
        // first icmp packet
        memcpy(str, "START", 5);
        *count = 0;
    }
    if (*count >= ICMP_LIMIT) {
        return *count;
    } else {
        *count += 1;
        return 0;
    }
}

int32_t check_pktlab_message_ndata(uint8_t sktid, uint8_t family, uint8_t proto,
                                   pktlab_time_t time, uint8_t raddrlen,
                                   const void* raddrptr, uint8_t rportlen,
                                   const void* rportptr, uint32_t len,
                                   void* persistent_memory,
                                   uint32_t persistent_memory_len) {
    if (proto == PKTLAB_RAW_PROTO) {
        return 0;
    }
    return 1 + proto;
}

int32_t check_pktlab_message_ncap(uint8_t sktid, uint8_t family, uint8_t proto,
                                  pktlab_time_t endtime,
                                  void* persistent_memory,
                                  uint32_t persistent_memory_len) {
    if (proto == PKTLAB_RAW_PROTO) {
        return 0;
    }
    return 1 + proto;
}
