comp_prog_digest:
	$(CC) -Wall -I$(PKTLAB_PKG_INC_PATH) -std=gnu99 $@.c $(PKTLAB_PKG_LIB_PATH) -o $@
