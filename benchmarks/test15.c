// Should remove -- dead slt instruction
// Range of the left operand totally to the left of the range of the
// right operand. Thus, the condition will always hold. The check
// should be removed.
int foo(int a, int b) {
  a = 1;
  b = 2147483647;
    
  if (a < b) { // Dead code (a = 1, b = 2147483647)
    a = a + b;
  }

  return a << b << (b >> 2);
}
