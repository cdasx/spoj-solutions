#include <stdio.h>
#include <math.h>

int main() {

    unsigned int t, n, x, a, b;

    scanf("%u", &t);

    while(t--) {
        scanf("%u", &n);
        x = ceil((sqrt(1 + 8 * n) - 1) / 2);
        if ((x & 1) == 0) {
            a = n - (x * (x - 1)) / 2;
            b = ((x + 1) * x) / 2 - n + 1;
        } else {
            a = ((x + 1) * x) / 2 - n + 1;
            b = n - (x * (x - 1)) / 2;
        }
        printf("TERM %u IS %u/%u\n", n, a, b);
    }

    return 0;
}
