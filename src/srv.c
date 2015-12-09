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
            request_quit = 1;
            c_log("SIGINT received");
            break;
        default:
            exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sig_handler);

    int listenfd = retrieve_listenfd();
    c_log("listenfd retrieved");

    int kqfd = retrieve_kqfd();
    c_log("kqfd retrieved");

    update_event(kqfd, listenfd, KREADEVENT, 0);

    mem_init();
    c_log("memory initialized");

    while (!request_quit)
        loop_once(kqfd, listenfd, 500);

    mem_free();
    c_log("memory released");

    c_log("quit");
    exit(EXIT_SUCCESS);
}
