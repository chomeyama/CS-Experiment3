#include <stdio.h>

int n;

int fact(int n) {
  int ret;
  if (n <= 0) {
    ret = 1;
  } else {
    ret = fact(n-1) * n;
  }
  return ret;
}

int main() {
  scanf("%d", &n);
  printf("%d\n", fact(n));
}
