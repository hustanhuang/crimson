#ifndef KQ_H
#define KQ_H

#define KREADEVENT 1
#define KWRITEEVENT 2
#define KMAXEVENTS 32

extern int retrieve_kqfd();

extern void update_event(int kqfd, int fd, int events, int modify);

extern void loop_once(int kqfd, int listen_fd, int interval);

#endif
