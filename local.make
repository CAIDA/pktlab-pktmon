CC           := $(WASI_SDK_PATH)/bin/clang --sysroot=$(WASI_SDK_PATH)/share/wasi-sysroot -I$(PKTLAB_PKG_INC_PATH) -I. -O3
IMPORT_FUNC  := import_func
LDFLAGS      := -Wl,--no-entry -Wl,--export-dynamic -Wl,--growable-table -Wl,--allow-undefined-file=$(IMPORT_FUNC) -Xlinker --import-memory=monitor,memory
AR           := $(WASI_SDK_PATH)/bin/llvm-ar
LD           := $(WASI_SDK_PATH)/bin/llvm-ld
MONITOR_NAME := $(MONITOR_NAME)

EXPORT_FUNC  := check_pktlab_message_nopen  \
				check_pktlab_message_nclose \
				check_pktlab_message_nsend  \
				check_pktlab_message_ndata  \
				check_pktlab_message_ncap   \
				malloc \
				free

OPT_EXPORT   := pktlab_message_nopen_result \
				pktlab_message_nclose_result \
				pktlab_message_nsend_result \
				pktlab_message_ncap_result

EXPORT_FUNC_LDFLAGS := $(foreach func,$(EXPORT_FUNC),-Wl,--export=$(func))
OPT_LDFLAGS  := $(foreach func,$(OPT_EXPORT),-Wl,--export-if-defined=$(func))

LDFLAGS      := $(LDFLAGS) $(EXPORT_FUNC_LDFLAGS) $(OPT_LDFLAGS)

MONITOR_SRC  := $(MONITOR_NAME).c

wasm: $(MONITOR_SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(MONITOR_NAME).wasm $(MONITOR_SRC) dummy.c

all: wasm
