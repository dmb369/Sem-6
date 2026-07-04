#include <stdio.h>
#define MAX 10
#define INFINITY 9999

void dijkstra(int cost[MAX][MAX], int nodes, int start) {
   int distance[MAX], visited[MAX], prev[MAX];
   int i, j, min, next;

   for (i = 0; i < nodes; i++) {
       distance[i] = cost[start][i];
       visited[i] = 0;
       prev[i] = (cost[start][i] != INFINITY && i != start) ? start : -1;
   }

   visited[start] = 1;
   for (i = 1; i < nodes; i++) {
       min = INFINITY;
       next = -1;
       for (j = 0; j < nodes; j++) {
           if (!visited[j] && distance[j] < min) {
               min = distance[j];
               next = j;
           }
       }

       if (next == -1) break;

       visited[next] = 1;
       for (j = 0; j < nodes; j++) {
           if (!visited[j] && cost[next][j] + distance[next] < distance[j]) {
               distance[j] = cost[next][j] + distance[next];
               prev[j] = next;
           }
       }
   }

   printf("\nShortest paths from node %d:\n", start);
   for (i = 0; i < nodes; i++) {
       printf("To node %d: cost = %d, path = ", i, distance[i]);
       int path[MAX], count = 0, temp = i;
       while (temp != -1) {
           path[count++] = temp;
           temp = prev[temp];
       }
       for (j = count - 1; j >= 0; j--) {
           printf("%d", path[j]);
           if (j != 0) printf(" -> ");
       }
       printf("\n");
   }
}

int main() {
   int nodes, i, j, start, cost[MAX][MAX];
   printf("Enter number of nodes: ");
   scanf("%d", &nodes);
   printf("Enter cost matrix (9999 for no link):\n");

   for (i = 0; i < nodes; i++) {
       for (j = 0; j < nodes; j++) {
           scanf("%d", &cost[i][j]);
       }
   }

   printf("Enter source node: ");
   scanf("%d", &start);
   dijkstra(cost, nodes, start);
 
   return 0;

}
