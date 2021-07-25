// Should remove -- dead sle instruction
// Ranges only overlap over one integer. That means that the
// conditional will always be true.
int foo() {
  int a, b;
  for (a = 0; a < 100; ++a);
  for (b = 400; b > 250; --b) {
    if (a <= b) { // Dead code (I[a] = [100, 250], 
                  //            I[b] = [250, 400])
      ++a;
    }
  }
  return a + b;
}
