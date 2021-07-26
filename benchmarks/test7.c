// Should not remove -- alive sge instruction
// Left limit of right operand is greater or equal than right limit
// of left operand
void foo(int a, int b) {
  a = 0;
  b = 63;
  while (a < 077) {
    a++;
    if (a >= b) { // Alive (a = 63, b = 63)
      printf("a sge b");
    }
  }
}
