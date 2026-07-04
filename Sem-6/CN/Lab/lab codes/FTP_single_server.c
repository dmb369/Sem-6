// ftp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int s, s2, t, n;
    struct sockaddr_in local, remote;
    char str[BUFFER_SIZE];

    // Create socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Server address setup
    bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind
    if (bind(s, (struct sockaddr *)&local, sizeof(local)) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen
    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    printf("File sharing server running on port %d...\n", PORT);
    t = sizeof(remote);
    s2 = accept(s, (struct sockaddr *)&remote, &t);
    if (s2 < 0) {
        perror("accept");
        exit(1);
    }

    printf("Client connected.\n");

    while ((n = recv(s2, str, BUFFER_SIZE, 0)) > 0) {
        str[n] = '\0';

        if (strncmp(str, "upload", 6) == 0) {
            // Get filename
            n = recv(s2, str, BUFFER_SIZE, 0);
            if (n <= 0) break;
            str[n] = '\0';
            FILE *fp = fopen(str, "wb");
            if (fp == NULL) {
                perror("fopen");
                break;
            }

            // Receive file data
            while ((n = recv(s2, str, BUFFER_SIZE, 0)) > 0) {
                if (strncmp(str, "EOF", 3) == 0) break;
                fwrite(str, sizeof(char), n, fp);
            }

            fclose(fp);
            printf("File uploaded.\n");
        }

        else if (strncmp(str, "download", 8) == 0) {
            // Get filename
            n = recv(s2, str, BUFFER_SIZE, 0);
            if (n <= 0) break;
            str[n] = '\0';

            FILE *fp = fopen(str, "rb");
            if (fp == NULL) {
                send(s2, "FILE_NOT_FOUND", 14, 0);
                continue;
            }

            while ((n = fread(str, sizeof(char), BUFFER_SIZE, fp)) > 0) {
                send(s2, str, n, 0);
            }

            fclose(fp);
            send(s2, "EOF", 3, 0);
            printf("File sent.\n");
        }

        else if (strncmp(str, "search", 6) == 0) {
            DIR *d;
            struct dirent *dir;
            d = opendir(".");
            if (d) {
                while ((dir = readdir(d)) != NULL) {
                    if (dir->d_type == DT_REG) {
                        snprintf(str, sizeof(str), "%s\n", dir->d_name);
                        send(s2, str, strlen(str), 0);
                        usleep(1000);
                    }
                }
                closedir(d);
                send(s2, "EOF", 3, 0);
            }
        }

        else {
            send(s2, "INVALID_COMMAND", 15, 0);
        }

        memset(str, 0, BUFFER_SIZE);
    }

    close(s2);
    close(s);
    return 0;
}
