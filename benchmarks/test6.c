// Should remove -- dead neq instruction
// Ranges of operands are the same
int foo(int a, int b) {
    a = 1;
    b = 1;
    
    if (a != b) { // Dead code
        return 0;
    }
    return 1;
}
