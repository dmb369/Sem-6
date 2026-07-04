// tcp_time_server.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5555

int main(void)
{
    int s, s2, t;
    struct sockaddr_in local, remote;
    char str[100];
    time_t current_time;
    struct tm * timeinfo;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    bzero((char *)&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *)&local, sizeof(local)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Time Server running on port %d...\n", PORT);

    for(;;) {
        t = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
            perror("accept");
            continue;
        }

        // get current time
        time(&current_time);
        timeinfo = localtime(&current_time);
        strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S\n", timeinfo);

        send(s2, str, strlen(str), 0);
        close(s2);
    }

    close(s);
    return 0;
}
