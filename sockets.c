/*sockets*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(void) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    struct sockaddr_in srv = {0};
    srv.sin_family = AF_INET;
    srv.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &srv.sin_addr);

    if (connect(s, (struct sockaddr*)&srv, sizeof srv) < 0) {
        perror("connect"); close(s); return 1;
    }

    const char *data = "hello";
    ssize_t w = write(s, data, strlen(data));
    if (w < 0) perror("write");

    char buf[512];
    memset(buf, 0, sizeof buf);
    ssize_t r = read(s, buf, sizeof buf - 1);
    if (r < 0) perror("read");

    close(s);
    printf("received: %s\n", buf);
    return 0;
}
