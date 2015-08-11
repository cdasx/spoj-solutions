#include<stdio.h>
#include<string.h>

#define MAXLEN 6100
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int cache[MAXLEN + 1][MAXLEN + 1];

char *strrev(char *str, unsigned int len) {
    char *r, *l;
    if (! str || ! *str) {
        return str;
    }
    for (r = str, l = str + len - 1; r < l; r++, l--) {
        *r ^= *l;
        *l ^= *r;
        *r ^= *l;
    }
    return str;
}

// lcslen using dp
unsigned int lcslen(char *a, char *b, int len_a, int len_b) {
    int i, j;
    for (i = len_a; i >= 0; i--) {
        for (j = len_b; j >= 0; j--) {
            if (a[i] == '\0' || b[j] == '\0') {
                cache[i][j] = 0;
            } else if (a[i] == b[j]) {
                cache[i][j] = 1 + cache[i + 1][j + 1];
            } else {
                cache[i][j] = MAX(cache[i + 1][j], cache[i][j + 1]);
            }
        }
    }
    return cache[0][0];
}

int main(int argc, char *argv[]) {

    int t, i, j, len = MAXLEN;
    char input[MAXLEN + 1], rev[MAXLEN + 1];

    scanf("%u", &t);

    while(t--) {

        scanf("%s", input);
        len = strlen(input);
        strcpy(rev, input);
        strrev(rev, len);

        printf("%u\n", len - lcslen(input, rev, len, len));
    }

    return 0;
}
