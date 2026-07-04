// udp_weather_server.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
    int s, t, n, cp;
    struct sockaddr_in local, remote;
    char str[100];

    if(argc != 2) {
        printf("Usage : server <port>\n");
        exit(0);
    }

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    bzero((char *)&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons((short)atoi(argv[1]));
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *)&local, sizeof(local)) == -1) {
        perror("bind");
        exit(1);
    }

    t = sizeof(remote);
    memset(str, 0, 100);

    for(;;) {
        if ((n = recvfrom(s, str, 100, 0, (struct sockaddr *)&remote, &t)) < 0) {
            perror("recv from");
            break;
        }

        if ((cp = fork()) == 0) {
            while (1) {
                // Prepare static weather info response
                snprintf(str, sizeof(str), "Weather: Sunny, 27°C\n");

                if (sendto(s, str, strlen(str), 0, (struct sockaddr *)&remote, sizeof(remote)) < 0) {
                    perror("send to");
                    break;
                }

                memset(str, 0, 100);

                // Exit after one reply to simulate a single-response server
                break;
            }
            exit(0);
        } else if (cp < 0) {
            perror("fork error");
            exit(1);
        }
    }

    close(s);
    return 0;
}
