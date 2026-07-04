#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define N 5

typedef struct {
    int matrix[N][N];
    int row_parity[N];
    int col_parity[N];
} Message;

void compute_parity(const int matrix[N][N], int row_parity[N], int col_parity[N]) {
    for (int i = 0; i < N; i++) {
        row_parity[i] = 0;
        col_parity[i] = 0;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            row_parity[i] ^= matrix[i][j];
            col_parity[j] ^= matrix[i][j];
        }
    }
}

void detect_error(const Message *M) {
    int computed_row_parity[N], computed_col_parity[N];
    compute_parity(M->matrix, computed_row_parity, computed_col_parity);

    int error_row = -1, error_col = -1;
    for (int i = 0; i < N; i++) {
        if (computed_row_parity[i] != M->row_parity[i]) {
            error_row = i;
        }
        if (computed_col_parity[i] != M->col_parity[i]) {
            error_col = i;
        }
    }

    if (error_row != -1 && error_col != -1) {
        printf("Error detected at position: (%d, %d)\n", error_row, error_col);
    } else {
        printf("No error detected.\n");
    }
}

int main(int argc, char *argv[]) {
    int s, n;
    struct sockaddr_in local, remote;
    socklen_t t;
    Message M;

    if (argc != 2) {
        printf("\nUsage: %s <server_port>\n", argv[0]);
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

    if (bind(s, (struct sockaddr*)&local, sizeof(local)) == -1) {
        perror("bind");
        exit(1);
    }

    t = sizeof(remote);
    printf("Server is ready and waiting...\n");

    while (1) {
        n = recvfrom(s, &M, sizeof(M), 0, (struct sockaddr *)&remote, &t);
        if (n < 0) {
            perror("recvfrom");
            exit(1);
        }

        printf("Matrix received from client:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", M.matrix[i][j]);
            }
            printf("\n");
        }

        detect_error(&M);
    }

    close(s);
    return 0;
}
