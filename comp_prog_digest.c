// program for quick ebpf program digest computation
//
// Compilation cmd:
//    gcc -Wall -I../include -std=gnu99 -L../lib comp_prog_digest.c -lpktlab -lpthread -ldl
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <pktlab.h>

char buffer[0x100000];

int main(int argc, char * argv[]) {
    FILE * fp;
    size_t rst, readlen;
    uint8_t digest[PKTLAB_SHA256_DIGEST_LEN];
    void * loaded_prog;
    void * encoded_prog;
    uint_fast32_t loaded_proglen, encoded_proglen, errlineno, i;

    if (argc != 2) {
        fputs("argc != 2\n", stderr);
        return -1;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("fopen");
        return -2;
    }

    readlen = 0;

    while (!feof(fp) && !ferror(fp)) {
        rst = fread(
            buffer+readlen, sizeof(buffer[0]),
            sizeof(buffer)-readlen, fp);
        readlen += rst;

        if (readlen >= sizeof(buffer)) {
            fputs("Too much data\n", stderr);
            return -3;
        }
    }

    if (ferror(fp)) {
        perror("fread");
        return -4;
    }

    // eof means all going well
    enum pktlab_prog_type prog_type;
    char* ptr = strrchr(argv[1], '.');
    if (ptr == NULL) {
        fputs("No suffix\n", stderr);
        return -4;
    }
    if (strcmp(ptr, ".ebpf") == 0) {
        prog_type = PKTLAB_EBPF;
    } else if (strcmp(ptr, ".wasm") == 0) {
        prog_type = PKTLAB_WASM;
    } else {
        fputs("Unknown suffix\n", stderr);
        return -4;
    }

    if (pktlab_load_program(
            buffer, readlen, prog_type,
            &loaded_prog, &loaded_proglen, &errlineno) < 0) {
        fputs("Load program failed\n", stderr);
        return -5;
    }

    if (pktlab_encode_program(
            prog_type, loaded_prog, loaded_proglen,
            &encoded_prog, &encoded_proglen) < 0) {
        fputs("Encode program failed\n", stderr);
        return -6;
    }

    if (pktlab_get_program_digest(
            encoded_prog, encoded_proglen, digest) < 0) {
        fputs("Compute program digest failed\n", stderr);
        return -7;
    }

    for (i = 0; i < sizeof(digest); ++i)
        printf("%02x", digest[i]);
    puts("");

    return 0;
}