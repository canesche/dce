// Should remove -- dead sge instruction
// Left limit of left operand is greater than right limit of right
// operand
void foo(int a, int b) {
  a = 0;
  b = 64;
  while (a < 077) {
    a++;
    if (a >= b) { // Dead code
      printf("a sge b");
    }
  }
}
