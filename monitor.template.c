// monitor.template.c
// Template monitor implementation

#include "monitor_helper.h"
#include "pktlab.h"

int32_t check_pktlab_message_nopen(uint8_t sktid, uint8_t family, uint8_t proto,
                                   uint32_t rbufsz, uint8_t laddrlen,
                                   uint8_t lportlen, uint8_t raddrlen,
                                   uint8_t rportlen, const void* laddrptr,
                                   const void* lportptr, const void* raddrptr,
                                   const void* rportptr,
                                   void* persistent_memory,
                                   uint32_t persistent_memory_len) {
    /**
     * return 0 if the message is valid, non-zero otherwise
     * For detailed return value rules, see README.md
     */

    /* Your code here */
    return -1;
}

/** Optional **/
void pktlab_message_nopen_result(uint8_t sktid, uint8_t family, uint8_t proto,
                                 uint32_t rbufsz, uint8_t laddrlen,
                                 uint8_t lportlen, uint8_t raddrlen,
                                 uint8_t rportlen, const void* laddrptr,
                                 const void* lportptr, const void* raddrptr,
                                 const void* rportptr, void* persistent_memory,
                                 uint32_t persistent_memory_len,
                                 enum pktlab_status errid) {
    // do nothing
}


int32_t check_pktlab_message_nclose(uint8_t sktid, void* persistent_memory,
                                    uint32_t persistent_memory_len) {
    /**
     * return 0 if the message is valid, non-zero otherwise
     * For detailed return value rules, see README.md
     */

    /* Your code here */
    return -1;
}

/** Optional **/
void pktlab_message_nclose_result(uint8_t sktid, void* persistent_memory,
                                  uint32_t persistent_memory_len,
                                  enum pktlab_status errid) {
    // do nothing
}


int32_t check_pktlab_message_nsend(uint8_t sktid, uint8_t family, uint8_t proto,
                                   pktlab_time_t time, uint16_t tag,
                                   uint8_t raddrlen, const void* raddrptr,
                                   uint8_t rportlen, const void* rportptr,
                                   uint32_t len, void* persistent_memory,
                                   uint32_t persistent_memory_len) {
    /**
     * return 0 if the message is valid, non-zero otherwise
     * For detailed return value rules, see README.md
     */

    /* Your code here */
    return -1;
}

/** Optional **/
void pktlab_message_nsend_result(uint8_t sktid, uint8_t family, uint8_t proto,
                                 pktlab_time_t time, uint16_t tag,
                                 uint8_t raddrlen, const void* raddrptr,
                                 uint8_t rportlen, const void* rportptr,
                                 uint32_t len, void* persistent_memory,
                                 uint32_t persistent_memory_len,
                                 enum pktlab_status errid) {
    // do nothing
}

int32_t check_pktlab_message_ndata(uint8_t sktid, uint8_t family, uint8_t proto,
                                   pktlab_time_t time, uint8_t raddrlen,
                                   const void* raddrptr, uint8_t rportlen,
                                   const void* rportptr, uint32_t len,
                                   void* persistent_memory,
                                   uint32_t persistent_memory_len) {
    /**
     * return 0 if the message is valid, non-zero otherwise
     * For detailed return value rules, see README.md
     */

    /* Your code here */
    return -1;
}

int32_t check_pktlab_message_ncap(uint8_t sktid, uint8_t family, uint8_t proto,
                                  pktlab_time_t endtime,
                                  void* persistent_memory,
                                  uint32_t persistent_memory_len) {
    /**
     * return 0 if the message is valid, non-zero otherwise
     * For detailed return value rules, see README.md
     */

    /* Your code here */
    return -1;
}

/** Optional **/
void pktlab_message_ncap_result(uint8_t sktid, uint8_t family, uint8_t proto,
                                pktlab_time_t endtime, void* persistent_memory,
                                uint32_t persistent_memory_len,
                                enum pktlab_status errid) {
    // do nothing
}
