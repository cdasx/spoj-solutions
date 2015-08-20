#include <stdio.h>
#include <string.h>

#define MAXLEN 100000

unsigned int permut[MAXLEN];

int main() {

    unsigned int n, i, is_amb;
    do {
        scanf("%u", &n);
        if (n > 0) {
            is_amb = 1;
            for (i = 0; i < n; ++i) {
                scanf("%u", permut + i);
            }
            for (i = 0; i < n; ++i) {
                if (i + 1 != permut[permut[i] - 1]) {
                    is_amb = 0;
                    break;
                }
            }
            if (is_amb == 1) {
                printf("ambiguous\n");
            } else {
                printf("not ambiguous\n");
            }
        }
    } while(n > 0);

    return 0;
}
