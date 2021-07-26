// Should remove -- dead sle instruction
// Ranges only overlap over one integer. That means that the
// conditional will always be true.
int foo() {
  int a, b;
  for (a = 0; a < 100; ++a);

  for (b = 400; b > 250; --b) {
    if (a <= b) { // Dead code (I[a] = [100, 250], 
                  //            I[b] = [251, 400])
                  // but the range Analyses gave: 
                  // I[a] = [100, +INF] 
                  // I[b] = [251, 400]
      ++a;        // so, for range Analyses isn't dead code
    }
  }
  return a + b;
}
