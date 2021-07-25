// Should not remove -- alive slt instruction
// I believe the innermost check is alive (i.e. cannot be removed
// safely)
int foo(int a, int b) {
  a = 1;
  b = 0;
  
  // This is some really crazy code.
  while (a < 500) {
    a++;
    b = 2147483647;
    while (b > 0) {
      b -= a;

      if (a < b) { // Probably alive?
        a = a + b;
      }
    }
  }

  return a << b << (b >> 2);
}
