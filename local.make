CC           := $(WASI_SDK_PATH)/bin/clang --sysroot=$(WASI_SDK_PATH)/share/wasi-sysroot -I$(PKTLAB_PKG_INC_PATH) -I. -O3
IMPORT_FUNC  := import_func
LDFLAGS      := -Wl,--no-entry -Wl,--export-dynamic -Wl,--growable-table -Wl,--allow-undefined-file=$(IMPORT_FUNC) -Xlinker --import-memory=monitor,memory
AR           := $(WASI_SDK_PATH)/bin/llvm-ar
LD           := $(WASI_SDK_PATH)/bin/llvm-ld
MONITOR_NAME := $(MONITOR_NAME)

EXPORT_FUNC  := -Wl,--export=check_pktlab_message_nopen  \
				-Wl,--export=check_pktlab_message_nclose \
				-Wl,--export=check_pktlab_message_nsend  \
				-Wl,--export=check_pktlab_message_ndata  \
				-Wl,--export=check_pktlab_message_ncap   \
				-Wl,--export=malloc \
				-Wl,--export=free

LDFLAGS      := $(LDFLAGS) $(EXPORT_FUNC)

MONITOR_SRC  := $(MONITOR_NAME).c

wasm: $(MONITOR_SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(MONITOR_NAME).wasm $(MONITOR_SRC) dummy.c

all: wasm
