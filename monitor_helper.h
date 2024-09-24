// monitor.h
//

#ifndef MONITOR_H
#define MONITOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// host callback
/**
 * This function is used to read the content (packet data and other fields
 * defined in virtual memory) from the virtual memory region.
 *
 * @param addr[in]: The address of the virtual memory region to read from.
 * @param buf[out]: The buffer to store the content read from the virtual
 * memory.
 * @param len[in]: The length of the content to read from the virtual memory.
 * @return: 0 on success, other values on failure.
 */
extern int read_vmem_region(uint32_t addr, void *buf, uint32_t len);
/**
 * This function flushes the persistent memory
 * region to the disk immediately. This is useful when you want to ensure that
 * the changes to the persistent memory are reflected on the disk. The persistent
 * memory will always be flushed to the disk when the monitor connection is
 * closed.
 *
 * @return: 0 on success, other values on failure.
 */
extern int persistent_memory_flush();


/**
 * This function is used to check if monitor debug mode is enabled.
 *
 * @return: 1 if debug mode is enabled, 0 otherwise.
 */
extern int endpoint_debug_enabled();

/**
 * This function is used to output debug information in the endpoint.
 * Only available when the monitor debug mode is enabled on the endpoint.
 * Otherwise, this function does nothing.
 * The current debug mode can be checked by calling endpoint_debug_enabled().
 *
 * @param str[in]: The string to output.
 * @return: void
 */
extern void monitor_debug(const char *str);

#define debug(...) do { \
    if (endpoint_debug_enabled()) { \
        char _buf[1024]; \
        snprintf(_buf, sizeof(_buf), __VA_ARGS__); \
        monitor_debug(_buf); \
    } \
} while (0)

#endif
