// Should not remove -- alive gtr instruction
// Right limit of left operand is greater than right limit of right
// operand
int foo() {
    int k = 0;
    while (k < 100) {
        k = k + 1;
    }
    if (k > 50) { // Alive (k = 100)
        printf("Hello World\n");
    }
    return 0;
}
