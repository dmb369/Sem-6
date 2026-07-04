#include <stdio.h>
#define MAX 10
#define INFINITY 9999

typedef struct {
   int distance[MAX];
   int next_hop[MAX];
} RoutingTable;

int main() {
   int cost[MAX][MAX], nodes, i, j, k;
   RoutingTable rt[MAX];
   printf("Enter number of nodes: ");
   scanf("%d", &nodes);
   printf("Enter cost matrix (9999 for no direct link):\n");
   for (i = 0; i < nodes; i++) {
       for (j = 0; j < nodes; j++) {
           scanf("%d", &cost[i][j]);
       }
   }
   // Initialize routing tables
   for (i = 0; i < nodes; i++) {
       for (j = 0; j < nodes; j++) {
           rt[i].distance[j] = cost[i][j];
           rt[i].next_hop[j] = j;
       }
   }
   int updated;
   do {
       updated = 0;
       for (i = 0; i < nodes; i++) {
           for (j = 0; j < nodes; j++) {
               for (k = 0; k < nodes; k++) {
                   if (rt[i].distance[j] > cost[i][k] + rt[k].distance[j]) {
                       rt[i].distance[j] = cost[i][k] + rt[k].distance[j];
                       rt[i].next_hop[j] = k;
                       updated = 1;
                   }
               }
           }
       }
   } while (updated);
 
   // Print final routing table
   for (i = 0; i < nodes; i++) {
       printf("\nRouting table for Node %d:\n", i);
       printf("Destination\tNext Hop\tDistance\n");
       for (j = 0; j < nodes; j++) {
           printf("%d\t\t%d\t\t%d\n", j, rt[i].next_hop[j], rt[i].distance[j]);
       }
   }
   return 0;
}

 

 

 


