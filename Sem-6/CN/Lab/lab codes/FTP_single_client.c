// ftp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

void upload_file(int s) {
    char str[BUFFER_SIZE];
    printf("Enter filename to upload: ");
    scanf("%s", str);

    FILE *fp = fopen(str, "rb");
    if (!fp) {
        perror("fopen");
        return;
    }

    send(s, "upload", 6, 0);
    sleep(1);
    send(s, str, strlen(str), 0);
    sleep(1);

    int n;
    while ((n = fread(str, sizeof(char), BUFFER_SIZE, fp)) > 0) {
        send(s, str, n, 0);
    }
    fclose(fp);
    send(s, "EOF", 3, 0);
    printf("File uploaded.\n");
}

void download_file(int s) {
    char str[BUFFER_SIZE];
    printf("Enter filename to download: ");
    scanf("%s", str);

    send(s, "download", 8, 0);
    sleep(1);
    send(s, str, strlen(str), 0);

    FILE *fp = fopen(str, "wb");
    if (!fp) {
        perror("fopen");
        return;
    }

    int n;
    while ((n = recv(s, str, BUFFER_SIZE, 0)) > 0) {
        if (strncmp(str, "EOF", 3) == 0) break;
        if (strncmp(str, "FILE_NOT_FOUND", 14) == 0) {
            printf("Server: File not found.\n");
            fclose(fp);
            remove(str);
            return;
        }
        fwrite(str, sizeof(char), n, fp);
    }

    fclose(fp);
    printf("File downloaded.\n");
}

void search_files(int s) {
    char str[BUFFER_SIZE];
    send(s, "search", 6, 0);
    int n;
    printf("Available files:\n");
    while ((n = recv(s, str, BUFFER_SIZE, 0)) > 0) {
        str[n] = '\0';
        if (strncmp(str, "EOF", 3) == 0) break;
        printf("%s", str);
    }
}

int main() {
    int s, t;
    struct sockaddr_in remote;
    char choice[10];

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    bzero(&remote, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORT);
    remote.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(s, (struct sockaddr *)&remote, sizeof(remote)) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, PORT);

    while (1) {
        printf("\nOptions:\n1. Upload\n2. Download\n3. Search\n4. Exit\n> ");
        scanf("%s", choice);

        if (strcmp(choice, "1") == 0) upload_file(s);
        else if (strcmp(choice, "2") == 0) download_file(s);
        else if (strcmp(choice, "3") == 0) search_files(s);
        else if (strcmp(choice, "4") == 0) {
            close(s);
            printf("Disconnected.\n");
            break;
        } else {
            printf("Invalid option.\n");
        }
    }

    return 0;
}
