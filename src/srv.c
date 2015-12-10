#include "include/srv.h"

#include <stdlib.h>
#include <signal.h>

#include "include/utils.h"
#include "include/kq.h"
#include "include/sock.h"
#include "include/mem.h"
#include "include/timed.h"

int mem_inited = 0,
    event_inited = 0;

void sig_handler(int sig) {
    switch (sig) {
        case SIGINT:
            c_log("SIGINT received");
            if (mem_inited) {
                mem_free();
                c_log("memory released");
            }
            if (event_inited) {
                event_free();
                c_log("events released");
            }
            c_log("quit");
            exit(EXIT_SUCCESS);
        default:
            exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sig_handler);

    int listenfd = retrieve_listenfd();
    c_log("listenfd ready");

    int kqfd = retrieve_kqfd();
    c_log("kqfd ready");

    update_event(kqfd, listenfd, KREADEVENT, 0);

    mem_inited = (mem_init() == 0);
    c_log("memory initialized");

    event_inited = (event_init() == 0);
    c_log("timed events list initialized");

    while (1)
        loop_once(kqfd, listenfd, 1000);
}
