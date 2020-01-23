#include <stdio.h>

int a[99], i, n;

void initialize() {
  int i;
  for (i = 2; i <= 100; i++) {
    a[i-2] = 0;
  }
}

void check(int p) {
  int i;
  i = p;
  while (i <= 100) {
    a[i-2] = 1;
    i = i + p;
  }
}

int main() {
  initialize();
  scanf("%d", &n);
  if (n <= 100) {
    for (i = 2; i <= n; i++) {
      if (a[i-2] == 0) {
        printf("%d\n", i);
        check(i);
      }
    }
  }
}
