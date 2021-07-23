int foo(int k) {
  int i = 0;
  if (k % 2) {
    i++;
  }
  return i & 1;
}
