// monitor.h
//

#ifndef MONITOR_H
#define MONITOR_H

#include <stddef.h>
#include <stdint.h>

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

#endif
