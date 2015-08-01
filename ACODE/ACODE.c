
#include <stdio.h>
#include <string.h>

#define MAXLEN 5000

int main() {
    char encoded[MAXLEN + 1];
    int i, len, val;
    long long int dp[MAXLEN + 1]; dp[0] = 1;
    for(;;) {
        scanf("%s", encoded);
        if(strcmp(encoded, "0") == 0) {
            break;
        }
        len = strlen(encoded);
        for(i = 0; i < len; i++) {
            if(i == 0) {
                dp[i + 1] = dp[i];
            } else {
                val = encoded[i] - '0' + 10 * (encoded[i - 1] - '0');
                if (encoded[i - 1] == '0') {
                    dp[i + 1] = dp[i];
                } else if (val == 10 || val == 20) {
                    dp[i + 1] = dp[i - 1];
                } else if (val < 27) {
                    dp[i + 1] = dp[i - 1] + dp[i];
                } else {
                    dp[i + 1] = dp[i];
                }
            }
        }
        printf("%lld\n", dp[i]);
    }
    return 0;
}
