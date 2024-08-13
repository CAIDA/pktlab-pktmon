comp_prog_digest:
	$(CC) -Wall -I../include -std=gnu99 -L../lib $@.c -lpktlab -lpthread -ldl -o $@
