#include "include/sock.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory.h>
#include <unistd.h>
#include <errno.h>

#include "include/config.h"
#include "include/utils.h"
#include "include/kq.h"
#include "include/parse.h"

int retrieve_listenfd() {

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    exit_if(listen_fd < 0);
    c_log("listenfd retrieved");

    SA_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(CRIMSON_PORT);
    c_log("server address initialized");

    exit_if(bind(listen_fd, (SA *)&addr, sizeof(addr)));
    c_log("binded");

    exit_if(listen(listen_fd, SOMAXCONN));
    c_log("listening");

    set_non_block(listen_fd);
    c_log("unblock listenfd");

    return listen_fd;
}

void handle_accept(int kqfd, int listen_fd) {
    c_log("accept event");
    SA_in cli_addr;
    socklen_t cli_size = sizeof(cli_addr);

    int cli_fd = accept(listen_fd, (SA *)&cli_addr, &cli_size);
    exit_if(cli_fd < 0);

    c_log("connection established");
    c_log(inet_ntoa(cli_addr.sin_addr));

    set_non_block(cli_fd);
    update_event(kqfd, cli_fd, KREADEVENT | KWRITEEVENT, 0);
}

void handle_read(int kqfd, int client_fd) {
    c_log("read event");
    char buff[MAX_BUFF_LEN];
    int n = 0;

    while ( (n = read(client_fd, buff, sizeof(buff))) > 0) {
        c_log("receive message");
        exit_if(parse(client_fd, buff, n));
        c_log("message processed");
    }

    if (n < 0 && errno == EAGAIN)
        return;
    exit_if(n < 0);
    c_log("connection terminated");
    close(client_fd);
}

void handle_write(int kqfd, int client_fd) {
    c_log("write event");
    update_event(kqfd, client_fd, KREADEVENT, 1);
}
