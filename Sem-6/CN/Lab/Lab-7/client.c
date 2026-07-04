// multi_client_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define SERVER_IP "127.0.0.1"  // Change this if server is remote
#define PORT 8080
#define BUFFER_SIZE 1024
 
void upload_file(int sockfd) {
   char filename[BUFFER_SIZE];
   char buffer[BUFFER_SIZE];
   int n;
 
   printf("Enter filename to upload: ");
   scanf("%s", filename);
 
   FILE *fp = fopen(filename, "rb");
   if (fp == NULL) {
       perror(strerror(errno));
       return;
   }
 
   // Tell server we want to upload
   send(sockfd, "upload", strlen("upload"), 0);
   sleep(1); // Small delay to avoid packet merging
 
   // Send filename
   send(sockfd, filename, strlen(filename), 0);
   sleep(1);
 
   // Send file content
   while ((n = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
       send(sockfd, buffer, n, 0);
   }
 
   fclose(fp);
   sleep(1);
 
   // Send EOF to indicate file transmission is complete
   send(sockfd, "EOF", strlen("EOF"), 0);
 
   printf("File upload completed.\n");
}
 
void download_file(int sockfd) {
   char filename[BUFFER_SIZE];
   char buffer[BUFFER_SIZE];
   int n;
 
   printf("Enter filename to download: ");
   scanf("%s", filename);
 
   // Tell server we want to download
   send(sockfd, "download", strlen("download"), 0);
   sleep(1); // Small delay
 
   // Send requested filename
   send(sockfd, filename, strlen(filename), 0);
 
   FILE *fp = fopen(filename, "wb");
   if (fp == NULL) {
       perror(strerror(errno));
       return;
   }
 
   while ((n = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
       buffer[n] = '\0';
       if (strncmp(buffer, "EOF", 3) == 0)
           break;
       if (strncmp(buffer, "FILE_NOT_FOUND", 14) == 0) {
           printf("Server: File not found.\n");
           fclose(fp);
           remove(filename); // Remove incomplete file
           return;
       }
       fwrite(buffer, sizeof(char), n, fp);
   }
 
   fclose(fp);
   printf("File download completed.\n");
}
 
int main() {
   int sockfd;
   struct sockaddr_in servaddr;
   char option[10];
 
   // Create socket
   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror(strerror(errno));
       exit(EXIT_FAILURE);
   }
 
   // Server address setup
   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(PORT);
 
   if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
       perror("Invalid address/ Address not supported");
       exit(EXIT_FAILURE);
   }
 
   // Connect to server
   if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
       perror(strerror(errno));
       exit(EXIT_FAILURE);
   }
 
   printf("Connected to server %s:%d\n", SERVER_IP, PORT);
 
   while (1) {
       printf("\nChoose an option:\n1. Upload file\n2. Download file\n3. Exit\nOption: ");
       scanf("%s", option);
 
       if (strcmp(option, "1") == 0) {
           upload_file(sockfd);
       }
       else if (strcmp(option, "2") == 0) {
           download_file(sockfd);
       }
       else if (strcmp(option, "3") == 0) {
           close(sockfd);
           printf("Disconnected from server.\n");
           break;
       }
       else {
           printf("Invalid option. Try again.\n");
       }
   }
 
   return 0;
}
 
 
