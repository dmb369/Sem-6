#include <stdio.h>

int max(int a, int b) 
{
    return (a > b) ? a : b;
}


int find(int a[], int n) 
{
    int l[n], r[n];

    
    for (int i = 0; i < n; i++) {
        l[i] = 1;
        r[i] = 1;
    }

    // Left
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (a[i] > a[j]) {
                l[i] = max(l[i], l[j] + 1);
            }
        }
    }

    // Right
    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            if (a[i] > a[j]) {
                r[i] = max(r[i], r[j] + 1);
            }
        }
    }

    // Max Calculation
    int m = 0;
    for (int i = 0; i < n; i++) {
        if (l[i] > 1 && r[i] > 1) {
            m = max(m, l[i] + r[i] - 1);
        }
    }

    return (m < 3) ? 0 : m;
}

int main() {
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int a[n];
    printf("Enter the elements:\n");
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    

    int ans = find(a, n);
    printf("%d\n", ans);
    
    return 0;
}
