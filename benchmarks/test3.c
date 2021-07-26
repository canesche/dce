// Should not remove -- alive eq instruction
// Ranges collapse
void foo(int a, int b) {
  a = 0;
  b = 0xFF;
  while (a < 0xFF) {
    ++a;
    while (b > 0) {
      if (a == b) // Alive (a = 1, b = 1)
        break;
      b--;
    }
  }
}
