// multi_client_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define PORT 8080
#define BUFFER_SIZE 1024
 
void sigchld_handler(int signo) {
   (void)signo; // To avoid unused parameter warning
   while (waitpid(-1, NULL, WNOHANG) > 0);
}
 
void handle_client(int connfd, int client_number) {
   char buffer[BUFFER_SIZE];
   int n;
 
   while ((n = recv(connfd, buffer, BUFFER_SIZE, 0)) > 0) {
       buffer[n] = '\0';
       
       printf("Client %d sent: %s\n", client_number, buffer);
 
       if (strncmp(buffer, "upload", 6) == 0) {
           // Expecting the next message to be filename
           n = recv(connfd, buffer, BUFFER_SIZE, 0);
           if (n <= 0) break;
           buffer[n] = '\0';
 
           char filename[BUFFER_SIZE];
           snprintf(filename, sizeof(filename), "Client%d-File-%d", client_number, 1); // For simplicity, always File-1. You can enhance.
 
           FILE *fp = fopen(filename, "wb");
           if (fp == NULL) {
               perror(strerror(errno));
               break;
           }
 
           // Receive file contents
           while ((n = recv(connfd, buffer, BUFFER_SIZE, 0)) > 0) {
               if (strncmp(buffer, "EOF", 3) == 0)
                   break;
               fwrite(buffer, sizeof(char), n, fp);
           }
 
           fclose(fp);
           printf("File uploaded as %s\n", filename);
       }
       else if (strncmp(buffer, "download", 8) == 0) {
           // Expecting filename to download
           n = recv(connfd, buffer, BUFFER_SIZE, 0);
           if (n <= 0) break;
           buffer[n] = '\0';
 
           FILE *fp = fopen(buffer, "rb");
           if (fp == NULL) {
               perror(strerror(errno));
               send(connfd, "FILE_NOT_FOUND", strlen("FILE_NOT_FOUND"), 0);
               continue;
           }
 
           while ((n = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
               send(connfd, buffer, n, 0);
           }
 
           fclose(fp);
           send(connfd, "EOF", strlen("EOF"), 0);
           printf("File %s sent to client %d\n", buffer, client_number);
       }
       else {
           send(connfd, "INVALID_COMMAND", strlen("INVALID_COMMAND"), 0);
       }
   }
}
 
int main() {
   int listenfd, connfd;
   struct sockaddr_in servaddr, cliaddr;
   socklen_t cliaddrlen;
   pid_t childpid;
   int client_counter = 0;
 
   signal(SIGCHLD, sigchld_handler); // Handle zombie processes
 
   // Create socket
   if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror(strerror(errno));
       exit(EXIT_FAILURE);
   }
 
   // Set socket options
   int opt = 1;
   if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
       perror(strerror(errno));
       exit(EXIT_FAILURE);
   }
 
   // Server address setup
   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port = htons(PORT);
 
   // Bind socket
   if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
       perror(strerror(errno));
       exit(EXIT_FAILURE);
   }
 
   // Listen
   if (listen(listenfd, 10) == -1) {
       perror(strerror(errno));
       exit(EXIT_FAILURE);
   }
 
   printf("Server listening on port %d...\n", PORT);
 
   for (;;) {
       cliaddrlen = sizeof(cliaddr);
       if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddrlen)) < 0) {
           perror(strerror(errno));
           continue;
       }
 
       client_counter++;
 
       if ((childpid = fork()) == 0) {
           close(listenfd); // Child doesn't need listening socket
           handle_client(connfd, client_counter);
           close(connfd);
           exit(0); // Important: child exits
       }
 
       close(connfd); // Parent closes connected socket
   }
 
   return 0;
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 