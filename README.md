# WebAssembly based monitor

## Prerequisites

- [WASI-SDK](https://github.com/WebAssembly/wasi-sdk): This link provides the instructions to install the WASI SDK (from pre-built binaries or from source).

WASI SDK is used to compile the C code into a WebAssembly binary, with the
`wasi-libc` library support for the memory manipulation functions. We won't use
any system calls provided by the `wasi-libc` due to security concerns.

## Build Instructions

Run `build.sh` to compile the C code into a WebAssembly binary.

The usage of the `build.sh` script is as follows:

```sh
# You should have the C code named `<name-of-monitor>.c` in the current directory
# Default name of the monitor is "monitor", which needs `monitor.c` to be present in the current directory
bash build.sh <path-to-wasi-sdk> <name-of-monitor(default to "monitor")>
```

For example, if the WASI SDK is installed in the `/opt/wasi-sdk` directory, and
you have a C code named `any_monitor.c`, then you can run the following command:

```sh
bash build.sh /opt/wasi-sdk any_monitor
```

This will generate a WebAssembly binary named `any_monitor.wasm` in the current
directory, and output the SHA256 digest of the monitor in the `any_monitor.digest` file.

Then, use `ppksman` to sign a new certificate with the monitor's digest, put the
certificate and the monitor in the correct directory under the controller side,
and then run the controller.

## Persistent memory database

The persistent memory database is used to store the persistent memory of the
monitors. By default, the persistent memory database is stored in the
`~/.pktlab/endpt.persistent` on the endpoint side. For each monitor, the
persistent memory is isolated by the unique key of the monitor(`cert_key_id+serial_number+index_in_cert`).

## How to write a monitor

### Monitor Template

The template for the monitor is provided in the `monitor.template.c` file. You
can start by copying the contents of this file into a new file, and then
implement the corresponding checks in the template.

Currently, the monitor is implemented to check the following packetlab messages
with the corresponding functions:

- `nopen`: `check_pktlab_message_nopen`
- `nclose`: `check_pktlab_message_nclose`
- `nsend`: `check_pktlab_message_nsend`
- `ndata`: `check_pktlab_message_ndata`
- `ncap`: `check_pktlab_message_ncap`

### Return Values

The monitor should return `0` if the packet is allowed, and a non-zero value if
the packet is not allowed. The non-zero value can be used to indicate the reason
for dropping the packet.

The valid non-zero values that used to indicate the reason for dropping the
packet should within `1 <= REASON_CODE <= UINT16_MAX`. We define the return type of the
check functions as `int32_t`, and the return value outside the range of `1 <= REASON_CODE <= UINT16_MAX`
will be treated as an error.

Specifically, the return value `rv` of the check functions should be:

1. `rv == 0`: The packet is allowed.
2. `1 <= rv < UINT16_MAX`: The packet is not allowed, and the reason for dropping the packet is `REASON_CODE`.
3. `rv >= UINT16_MAX`: The packet is not allowed, and the reason for dropping the packet is an error.

If the return value comes from controller check functions, as known as non-`ndata` messages, the endpoint will
return a monitor reject with error id 0 or monitor unknown (if `report_crash` is configured on endpoint) error to the controller.

If the return value comes from `ndata` message, the endpoint will drop the packet and send a `nblock` message to the controller (if `report_crash` is configured on endpoint) with the corresponding monitor information.

4. `rv < 0`: The packet is not allowed, and the reason for dropping the packet is an error.

The endpoint will return a monitor reject with error id 0 if the message is not
`ndata` and will just drop the packet if the message is `ndata`.

### Helper Functions

Some helper functions are provided in the `monitor_helper.h` file, which can be
used in the monitor implementation. The following helper functions are provided:

```c
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
```

```c
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
```

### Example Monitors

Some example monitors are provided in the `example_monitors` directory. You can
use these monitors as a reference to write your own monitor.

1. `icmp_limit_monitor.c`: This monitor is used to limit the number of ICMP
   packets sent by the user program. The user program can send a maximum of `10`
   ICMP packets, and the monitor will drop any ICMP packet sent after the limit
   is reached. No matter whether the controller is restarted or the endpoint is
   restarted, the monitor will keep the state of the number of ICMP packets sent
   based on the persistent memory.

2. `http_monitor.c`: This monitor will only allow the user program to send HTTP
   requests and receive HTTP responses. The monitor is designed drop any packet
   that is not an HTTP request or response. However, to simplify the implementation,
   the monitor will only check request methods and "HTTP" in the response status line.
   So, strictly, this monitor is not designed to be used in a real-world scenario,
   it is just an example to show how to write a monitor.

Use the following commands to build the example monitors:

```sh
bash build.sh <path-to-wasi-sdk> examples/icmp_limit_monitor
bash build.sh <path-to-wasi-sdk> examples/http_monitor
```

### Test Your Monitor
To test your monitor without generating a new certificate, you can copy your
compiled monitor to the `monitor_wasm` directory and rename it to `test.wasm`.

Then, recompile the `pktlabme` with `-DTEST_MONITOR` flag, and run the
`pktlabme` under the parent directory of `monitor_wasm` directory (which should
be the root directory of the `pktlabme`).

The `test.wasm` monitor will be put at the head of the monitor list, and other
monitors that receive from the controller will be put after the `test.wasm`
monitor.
