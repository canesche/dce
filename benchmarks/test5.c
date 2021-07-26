// Should remove -- dead eq instruction
// Disjunct operand ranges
void foo(int a, int b) {
    a = 0;
    b = 60;
    
    while (b > 53) {
        b--;
        while (a < 52) {
            a++;
            if (a == b) { // Dead code
                printf("loooll\n");
                break;
            }
        }
    }
}
