#include "include/srv.h"

#include <stdlib.h>
#include <signal.h>

#include "include/utils.h"
#include "include/kq.h"
#include "include/sock.h"
#include "include/db.h"
#include "include/timed.h"

int event_inited = 0;
int db_inited = 0;

void sig_handler(int sig) {
    switch (sig) {
        case SIGINT:
            c_log("SIGINT received");
            if (event_inited) {
                event_free();
                c_log("events released");
            }
            if (db_inited) {
                db_destroy();
                c_log("db destroyed");
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

    db_inited = (db_init() == 0);
    c_log("db initialized");

    event_inited = (event_init() == 0);
    c_log("timed events list initialized");

    while (1)
        loop_once(kqfd, listenfd, 1000);
}
