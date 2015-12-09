#include "include/srv.h"

#include <stdlib.h>
#include <signal.h>

#include "include/utils.h"
#include "include/kq.h"
#include "include/sock.h"
#include "include/mem.h"

void sig_handler(int sig) {
    switch (sig) {
        case SIGINT:
            c_log("SIGINT received");
            mem_free();
            c_log("memory released");
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

    mem_init();
    c_log("memory initialized");

    while (1)
        loop_once(kqfd, listenfd, 1000);
}
