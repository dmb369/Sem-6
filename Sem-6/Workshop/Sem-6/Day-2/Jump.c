#include <stdio.h>

int farthest(int a[], int n, int b, int l) {
    int largest[l + 1]; // Array to store the largest height differences covered by ladders
    int count = 0;    

    for (int i = 0; i < n - 1; i++) {
        int diff = a[i + 1] - a[i];

        if (diff > 0) {
            if (count < l) 
            {
                // Use a ladder for this height difference
                largest[count++] = diff;
            } 

            else 
            {
                // Find the smallest difference currently covered by a ladder
                int smallest = 0;
                for (int j = 1; j < count; j++)
                {
                    if (largest[j] < largest[smallest]) 
                    {
                        smallest = j;
                    }
                }

                if (count > 0 && diff > largest[smallest]) 
                {
                    // Replace the smallest ladder difference with the current one
                    b -= largest[smallest];
                    largest[smallest] = diff;
                } 

                else 
                {
                    // Use blocks for this difference
                    b -= diff;
                }
            }

            if (b < 0) return i;         
        }
    }

    return n - 1; 
}

int main() 
{
    int n, b, l;

    printf("Enter the number of towers: ");
    scanf("%d", &n);

    int a[n];
    printf("Enter the a of the towers: ");
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);

    printf("Blocks and Ladders:");
    scanf("%d %d", &b, &l);

    int result = farthest(a, n, b, l);
    printf("The farthest tower that can be reached is: %d\n", result);

    return 0;
}
