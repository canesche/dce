// Should remove -- dead gtr instruction
// Range of the left operand is wholly to the left of the right
// operand
int foo() {
    int k = 0;
    while (k < 100) {
        k = k + 1;
    }
    k = 0;
    if (k > 200) { // Dead code
        printf("Hello World\n");
    }
    return 0;
}
