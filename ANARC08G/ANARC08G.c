#include <stdio.h>
#include <stdlib.h>

int main () {
  unsigned int n, i = 0, j, k, amount, total, min_total;
  int *balances;
  scanf("%u", &n);
  while (n > 0) {
    i++;
    balances = (int *)calloc(n, sizeof(int));
    total = 0;
    for (j = 0; j < n; ++j) {
      for (k = 0; k < n; ++k) {
        scanf("%u", &amount);
        balances[j] -= amount;
        balances[k] += amount;
        total += amount;
      }
    }
    for (j = 0, min_total = 0; j < n; ++j) {
      min_total += (balances[j] > 0 ? balances[j] : 0);
    }
    printf("%d. %u %u\n", i, total, min_total);
    free(balances);
    scanf("%u", &n);
  }
  return 0;
}
