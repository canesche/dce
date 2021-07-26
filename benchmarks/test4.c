// Should remove -- dead eq instruction
// Disjunct operand ranges
void foo(int a, int b) {
    a = 0;
    b = 50;
    while (a < 52) {
        a++;
    }
    while (b > 10) {
        b--;
        if (a == b) { // Dead code
            printf("loooll\n");
            break;
        }
    }
}
