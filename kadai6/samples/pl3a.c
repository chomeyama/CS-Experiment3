#include <stdio.h>

int i, j, n, a[100];

void initialize(int n) {
  int i;
  for (i = 1; i <= n; i++) {
    scanf("%d", &a[i-1]);
  }
}

void swap(int j) {
  int temp;
  temp = a[j-1];
  a[j-1] = a[j+1-1];
  a[j+1-1] = temp;
}

int main() {
  scanf("%d", &n);
  if (n <= 100) {
    initialize(n);
    i = n;
    while (1 <= i) {
      j = 1;
      while (j < i) {
        if (a[j-1] > a[j+1-1]) {
          swap(j);
        }
        j++;
      }
      printf("%d\n", a[i-1]);
      i--;
    }
  }
}
