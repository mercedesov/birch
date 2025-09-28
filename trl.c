
/* trl.c - timed read line */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

/* timed read line */
char *trl (int timeout)
{
    static char buf[512];
    fd_set rfds;
    struct timeval tv;
    int ret;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    ret = select(1, &rfds, 0, 0, &tv);
    if (ret && FD_ISSET(0, &rfds))
    {
        memset(buf, 0, 512);
        ret = read(0, buf, 511);
        if (ret < 1)
            return 0;
        ret--;
        buf[ret] = 0;

        return buf;
    }
    else
        return 0;
}

int main()
{
    char *name;

    printf("What is your name? Think fast!\n");
    name = trl(3);
    if (name)
        printf("Hello %s\n", name);
    else
        printf("Too slow!\n");

    return 0;
}
