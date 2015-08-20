#include <stdio.h>
#include <stdlib.h>

#define MAXO 1000

int o[MAXO];

int m(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

int main(int argc, char *argv[]) {

    int t, g, n, i, j, c, f;
    scanf("%d", &t);

    for (j = 0; j < t; j++) {
        c = 0; f = 0;
        scanf("%d%d", &g, &n);
        for (i = 0; i < n; ++i) {
            scanf("%d", o + i);
        }
        qsort(o, n, sizeof(int), m);
        for(i = 0; i < n; ++i) {
            c++;
            g -= o[i];
            if (g <= 0) {
                f = 1;
                break;    
            }
        }
        printf("Scenario #%d:\n", j + 1);
        if (f == 0) {
            printf("impossible\n");
        } else {
            printf("%d\n", c);
        }
    }
    
    return 0;
}
