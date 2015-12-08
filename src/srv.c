#include <stdlib.h>
#include <signal.h>

#include "utils.h"
#include "kq.h"
#include "sock.h"

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

    do {
        loop_once(kqfd, listenfd, 500);
    } while (!request_quit);

    c_log("quit");
    exit(EXIT_SUCCESS);
}
