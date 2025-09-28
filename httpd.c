/* webserver */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define LISTENADDR "0.0.0.0"

struct HttpRequest {
    char method[8];
    char url[128];
};
typedef struct HttpRequest HttpRequest;

struct File {
    char filename[64];
    char *fc;
    int size;
};
typedef struct File File;

char *error;

int srv_init(int portno) {
    int s;
    struct sockaddr_in srv;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        error = "socket() error";
        return 0;
    }

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr(LISTENADDR);
    srv.sin_port = htons(portno);

    if (bind(s, (struct sockaddr *)&srv, sizeof(srv)) < 0) {
        close(s);
        error = "bind() error";
        return 0;
    }

    if (listen(s, 5) < 0) {
        close(s);
        error = "listen() error";
        return 0;
    }

    return s;
}

int cli_accept(int s) {
    int c;
    socklen_t addrlen;
    struct sockaddr_in cli;

    addrlen = sizeof(cli);
    memset(&cli, 0, sizeof(cli));
    c = accept(s, (struct sockaddr *)&cli, &addrlen);
    if (c < 0) {
        error = "accept() error";
        return 0;
    }

    return c;
}

HttpRequest *parse_http(char *str) {
    HttpRequest *req;
    char *p;

    req = malloc(sizeof(HttpRequest));

    for (p = str; *p && *p != ' '; p++);
    if (*p == ' ')
        *p = 0;
    else {
        error = "parse_http() NOSPACE error";
        free(req);
        return 0;
    }
    strncpy(req->method, str, 7);

    for (str = ++p; *p && *p != ' '; p++);
    if (*p == ' ')
        *p = 0;
    else {
        error = "parse_http() 2NDSPACE error";
        free(req);
        return 0;
    }

    strncpy(req->url, str, 127);
    return req;
}

char *cli_read(int c) {
    static char buf[512];
    memset(buf, 0, 512);
    if (read(c, buf, 511) < 0) {
        error = "read() error";
        return 0;
    } else
        return buf;
}

void http_headers(int c, int code, const char *ctype, int len) {
    char buf[512];
    snprintf(buf, 511,
        "HTTP/1.0 %d OK\r\n"
        "Server: httpd.c\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n",
        code, ctype, len);
    write(c, buf, strlen(buf));
}

void http_response(int c, int code, const char *ctype, const char *data) {
    http_headers(c, code, ctype, strlen(data));
    write(c, data, strlen(data));
}

File *readfile(char *filename) {
    int n, x, fd;
    File *f;
    char buf[512];

    fd = open(filename, O_RDONLY);
    if (fd < 0) return 0;

    f = malloc(sizeof(File));
    if (!f) {
        close(fd);
        return 0;
    }

    strncpy(f->filename, filename, 63);
    f->fc = malloc(1);
    f->fc[0] = 0;

    x = 0;
    while (1) {
        n = read(fd, buf, 512);
        if (n <= 0) break;
        f->fc = realloc(f->fc, x + n);
        memcpy(f->fc + x, buf, n);
        x += n;
    }

    f->size = x;
    close(fd);

    return f;
}

void sendfile_http(int c, const char *ctype, File *file) {
    if (!file) {
        http_response(c, 404, "text/plain", "File not found");
        return;
    }

    http_headers(c, 200, ctype, file->size);
    write(c, file->fc, file->size);

    free(file->fc);
    free(file);
}

void cli_conn(int s, int c) {
    HttpRequest *req;
    char *p;
    File *f;

    p = cli_read(c);
    if (!p) {
        fprintf(stderr, "%s\n", error);
        close(c);
        return;
    }

    req = parse_http(p);
    if (!req) {
        fprintf(stderr, "%s\n", error);
        close(c);
        return;
    }

    if (!strcmp(req->method, "GET") && !strcmp(req->url, "/")) {
        const char *res = "<html><body><h1>Hello, World!</h1>"
                          "<img src='/img/test.png'></body></html>";
        http_response(c, 200, "text/html", res);
    } else if (!strcmp(req->method, "GET") && !strncmp(req->url, "/img/", 5)) {
        char path[128];
        snprintf(path, sizeof(path), ".%s", req->url);
        f = readfile(path);
        sendfile_http(c, "image/png", f);
    } else {
        http_response(c, 404, "text/plain", "File not found");
    }

    free(req);
    close(c);
}

int main(int argc, char *argv[]) {
    int s, c;
    char *port;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return -1;
    } else
        port = argv[1];

    s = srv_init(atoi(port));
    if (!s) {
        fprintf(stderr, "%s\n", error);
        return -1;
    }

    printf("Listening on %s:%s\n", LISTENADDR, port);
    while (1) {
        c = cli_accept(s);
        if (!c) {
            fprintf(stderr, "%s\n", error);
            continue;
        }

        if (!fork()) {
            cli_conn(s, c);
            exit(0);
        }
        close(c);
    }

    return 0;
}
