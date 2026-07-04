// tcp_time_client.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5555

int main(int argc, char * argv[])
{
    int s, t;
    struct sockaddr_in remote;
    char str[100];

    if(argc != 2){
        printf("\nInvalid server address\n");
        exit(0);
    }

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect to %s...\n", argv[0]);

    bzero((char *)&remote, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORT);
    remote.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(s, (struct sockaddr *)&remote, sizeof(remote)) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected.\n");

    t = recv(s, str, sizeof(str) - 1, 0);
    if (t > 0) {
        str[t] = '\0';
        printf("Server time: %s", str);
    } else {
        if (t < 0) perror("recv");
        else printf("Server closed connection\n");
        exit(1);
    }

    close(s);
    return 0;
}
