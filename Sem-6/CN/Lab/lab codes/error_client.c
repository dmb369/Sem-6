#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <netinet/in.h>

#define N 5

typedef struct {
    int matrix[N][N];
    int row_parity[N];
    int col_parity[N];
} Message;

void generate_matrix(int matrix[N][N]) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 2; 
        }
    }
}

void display_matrix(const int matrix[N][N]) {
    printf("Matrix before inducing error:\n");
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void compute_parity(const int matrix[N][N], int row_parity[N], int col_parity[N]) {
    for(int i = 0; i < N; i++) {
        row_parity[i] = 0;
        col_parity[i] = 0;
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            row_parity[i] ^= matrix[i][j];
            col_parity[j] ^= matrix[i][j];
        }
    }
}

void induce_error(Message *M) {
    int error_row = rand() % N;
    int error_col = rand() % N;
    printf("\nInducing error at position (%d, %d)\n", error_row, error_col);
    M->matrix[error_row][error_col] ^= 1; // induce the error
}

int main(int argc, char *argv[]) {
    int s;
    struct sockaddr_in remote;
    int t;

    if (argc != 3) {
        printf("\nUsage: %s <server_ip> <server_port>\n", argv[0]);
        exit(0);
    }

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    bzero((char *)&remote, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons((short)atoi(argv[2]));
    remote.sin_addr.s_addr = inet_addr(argv[1]);

    t = sizeof(remote);

    Message M;
    srand(time(0)); // Random seed
    generate_matrix(M.matrix);
    display_matrix(M.matrix);  // Display matrix before error
    compute_parity(M.matrix, M.row_parity, M.col_parity);
    induce_error(&M);

    if (sendto(s, &M, sizeof(M), 0, (struct sockaddr *)&remote, sizeof(remote)) < 0) {
        perror("sendto");
        exit(1);
    }

    printf("\nMatrix with induced error sent to server successfully.\n");

    close(s);
    return 0;
}
