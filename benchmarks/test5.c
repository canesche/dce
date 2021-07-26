// Should not remove -- alive neq instruction
// Ranges of operands collapse over more than one number
int foo(int a, int b) {
    a = 0;
    b = 60;
    
    if (a != b) { // Alive (a = 0, b = 60)
        return 0;
    }
    return 1;
}
