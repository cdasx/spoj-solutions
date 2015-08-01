#include <stdio.h>

#define MAXLEN 10000
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(int argc, char *argv[]) {

    int k, i, j, n;
    long long int coins[MAXLEN], dp[MAXLEN + 2];

    scanf("%d", &k);
    for(j = 0; j < k; ++j) {
        scanf("%d", &n);
        dp[0] = 0; dp[1] = 0;
        for(i = 0; i < n; ++i) {
            scanf("%lld", coins + i);
            dp[i + 2] = MAX(dp[i] + coins[i], dp[i + 1]);
        }
        printf("Case %d: %lld\n", j + 1, dp[i + 1]);
    }

    return 0;
}
