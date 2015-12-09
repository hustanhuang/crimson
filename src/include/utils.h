#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>

#define exit_if(condition) do { if (condition) { perror(__FUNCTION__); exit(EXIT_FAILURE); } } while (0)
#define c_log(msg) do { \
    struct timeval tick; \
    gettimeofday(&tick, NULL); \
    printf("%.19s.%03ld * %s\n", \
            ctime(&tick.tv_sec), \
            tick.tv_usec / 1000, \
            msg); \
} while (0)

static int request_quit = 0;

static void set_non_block(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    exit_if(flags < 0);
    exit_if(fcntl(fd, F_SETFL, flags | O_NONBLOCK));
}

static void c_prt(const void *p, size_t len) {
    const char *s = (const char *)p;
    printf("%lu\t", len);
    for (size_t i = 0; i != len; ++i) {
        if (isprint(s[i]))
            putchar(s[i]);
        else
            printf("\033\[31m\\%02X\033\[0m", s[i]);
    }
    putchar('\n');
}

#endif
