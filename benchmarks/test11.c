// Should not remove -- alive sle instruction
// Range of the two operands overlap
int foo() {
  int a, b;
  for (a = 0; a < 100; ++a);
  for (b = 400; b > -100; --b);
  
  return a <= b ? 0 : 1; // Alive conditional
}
