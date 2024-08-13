# PacketLab Monitor Builder (`pktmon`)
> For building pktlab monitors!

## Prerequisites

- `libpktlab` with SSL support. This can be obtained by following the instructions in [installing the PacketLab software package](https://packetlab.github.io/tutorial/installation/) (from pre-built binaries or from source).
- The latest release of [WASI-SDK](https://github.com/WebAssembly/wasi-sdk), where this link provides the instructions to install the WASI SDK (from pre-built binaries or from source).
> WASI SDK is used to compile the monitor C code into a WebAssembly binary (i.e. the monitor binary), with the `wasi-libc` library support for the memory manipulation functions. We won't use any system calls provided by the `wasi-libc` due to security concerns.

## Compile Monitor Instructions

Run `pktmon.sh` to compile the C code into a monitor binary.

The usage of the `pktmon.sh` script is as follows:

```sh
# You should have the C code named `<name-of-monitor>.c` in the current directory
# Default name of the monitor is "monitor", which needs `monitor.c` to be present in the current directory
bash pktmon.sh <path-to-pktlab-pkg> <path-to-wasi-sdk> <name-of-monitor(default to "monitor")>
```

For example, if the PacketLab software package is installed at the `/opt/pktlab` directory,
WASI SDK is installed at the `/opt/wasi-sdk` directory,
and you have a monitor C code named `any_monitor.c`, then you can run the following command:

```sh
bash pktmon.sh /opt/pktlab /opt/wasi-sdk any_monitor
```

This will generate a monitor binary named `any_monitor.wasm` in the current
directory, and output the SHA256 digest of the monitor in the `any_monitor.digest` file.

You can then use `ppksman` from the `pktlab` Python3 package to sign a new `exppriv` or `delpriv` certificate with the monitor's digest, which you can then distribute with the compiled monitor to expermenters for them to use to contact your pktlab endpoint.
<!--put the certificate and the monitor in `~/.pktlab/certs` and `~/.pktlab/progs`, respectively, configure `xpmgr.conf` under the `~/.pktlab` to use the certificate, and the run `pktxpmgr` to perform measurements.-->

## How to Write a Custom Monitor

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

### Function Return Values

The monitor check functions's return value will determine if the request/notification message is allowed or not.
Specifically, the return value `rv` of the check functions should be:

1. `rv == 0`: The message is allowed.
2. `1 <= rv < UINT16_MAX`: The message is not allowed, and `rv` is the reason for dropping the message.
3. `rv >= UINT16_MAX`: The message is not allowed, and the reason for dropping the message is an error in the monitor.
4. `rv < 0`: The message is not allowed, and the reason for dropping the packet is intentionally hidden by the monitor.

For not allowed requests (non-`ndata` messages for now), a monitor reject (`PKTLAB_ECTLMONREJ`) result message is returned when `1 <= rv < UINT16_MAX` (`rv` returned as `errid`), or `rv < 0` (`errid` is 0), or `rv >= UINT16_MAX` when the endpoint is not configured to report crash (`report_crash` for reference endpoint `pktendpt`/`pktlabme`; `errid` is 0);
a monitor unknown (`PKTLAB_ECTLMONUKN`) result message is returned when `rv >= UINT16_MAX` and the endpoint is configured to report crash.

For not allowed notifications (`ndata` message for now), a nblock notification is returned when `1 <= rv < UINT16_MAX` (`rv` returned as `monerrid`) or when `rv >= UINT16_MAX` and the endpoint is configured to report crash (`monerrid` is 0).
The not allowed notifications are otherwise just dropped silently.

The rejecting/complaining monitor indx is always returned when a monitor reject/monitor unknown/nblock message is returned.

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
   packets sent by the controller. The controller can send a maximum of `10`
   ICMP packets, and the monitor will drop any ICMP packet sent after the limit
   is reached. No matter whether the controller is restarted or the endpoint is
   restarted, the monitor will keep the state of the number of ICMP packets sent
   based on the persistent memory.

2. `http_monitor.c`: This monitor will only allow the controller to send HTTP
   requests and receive HTTP responses. The monitor is designed to drop any packet
   that is not an HTTP request or response. However, to simplify the implementation,
   the monitor will only check request methods and "HTTP" in the response status line.
   So, strictly, this monitor is not designed to be used in a real-world scenario,
   it is just an example to show how to write a monitor.

3. `tcp_only_monitor.c`: This monitor will only allow the controller to communicate over TCP.

4. `pass_all_monitor.c`: This monitor will allow everything.

Use the following commands to build the example monitors:

```sh
bash pktmon.sh <path-to-pktlab-pkg> <path-to-wasi-sdk> examples/icmp_limit_monitor
bash pktmon.sh <path-to-pktlab-pkg> <path-to-wasi-sdk> examples/http_monitor
bash pktmon.sh <path-to-pktlab-pkg> <path-to-wasi-sdk> examples/tcp_only_monitor
bash pktmon.sh <path-to-pktlab-pkg> <path-to-wasi-sdk> examples/pass_all_monitor
```

<!--
### Test Your Monitor
To test your monitor without generating a new certificate, you can copy your
compiled monitor to the `monitor_wasm` directory and rename it to `test.wasm`.

Then, recompile the `pktlabme` with `-DTEST_MONITOR` flag, and run the
`pktlabme` under the parent directory of `monitor_wasm` directory (which should
be the root directory of the `pktlabme`).

The `test.wasm` monitor will be put at the head of the monitor list, and other
monitors that receive from the controller will be put after the `test.wasm`
monitor.
-->