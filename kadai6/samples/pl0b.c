#include <stdio.h>

int n, x;

void prime() {
  int m;
  m = x / 2;
  while (x != (x/m)*m) {
    m = m-1;
  }
  if (m == 1) {
    printf("%d\n", x);
  }
}

int main() {
  scanf("%d", &n);
  while (1 < n) {
    x = n;
    prime();
    n = n-1;
  }
}
