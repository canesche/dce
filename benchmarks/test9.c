// Should remove -- dead sge instruction
// Range of left operand indicates that it is always greater or
// equal than the right operand. Thus, performing the check is
// unnecessary.
int foo(int a, int b) {
    a = 0;
    b = 60;
    
    if (b >= a) {
        return 0;
    }
    return 1;
}
