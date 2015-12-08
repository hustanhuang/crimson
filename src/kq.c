#include "include/kq.h"

#include <sys/event.h>

#include "include/utils.h"
#include "include/sock.h"

int retrieve_kqfd() {
    int epollfd = kqueue();
    exit_if(epollfd < 0);
    return epollfd;
}

void update_event(int kqfd, int fd, int events, int modify) {
    struct kevent ev[2];
    int n = 0;
    if (events & KREADEVENT) {
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, (void *)(intptr_t) fd);
    } else if (modify) {
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, (void *)(intptr_t) fd);
    }
    if (events & KWRITEEVENT) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, (void *)(intptr_t) fd);
    } else if (modify) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, (void *)(intptr_t) fd);
    }
    exit_if(kevent(kqfd, ev, n, NULL, 0, NULL));
}

void loop_once(int kqfd, int listen_fd, int interval) {
    if (request_quit)
        return;

    /* process timed events here */

    struct timespec timeout;
    timeout.tv_sec = interval / 1000;
    timeout.tv_nsec = (interval % 1000) * 1000 * 1000;

    struct kevent active_events[KMAXEVENTS];
    int n = kevent(kqfd, NULL, 0, active_events, KMAXEVENTS, &timeout);

    for (int i = 0; i != n; ++i) {

        int client_fd = (int)(intptr_t) active_events[i].udata;
        int events = active_events[i].filter;

        if (events == EVFILT_READ) {
            if (client_fd == listen_fd) {
                handle_accept(kqfd, listen_fd);
            } else {
                handle_read(kqfd, client_fd);
            }
        } else if (events == EVFILT_WRITE) {
            handle_write(kqfd, client_fd);
        } else {
            exit_if(1);
        }
    }
}
