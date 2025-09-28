/* srv */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main(void) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); exit(1); }

    struct sockaddr_in srv = {0};
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr*)&srv, sizeof srv) < 0) {
        perror("bind"); close(s); exit(1);
    }

    if (listen(s, 5) < 0) {
        perror("listen"); close(s); exit(1);
    }

    printf("Listening on 0.0.0.0:%d\n", PORT);

    for (;;) {
        int c = accept(s, NULL, NULL);
        if (c < 0) { perror("accept"); continue; }

        char buf[512];
        memset(buf, 0, sizeof buf);
        read(c, buf, sizeof buf - 1);

        const char *data = "httpd v1.0\n";
        write(c, data, strlen(data));

        close(c);
    }

    close(s);
    return 0;
}
