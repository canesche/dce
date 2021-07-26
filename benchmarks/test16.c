// Should remove -- trivially dead code
// Instruction doesn't do anything, trivially!
int foo(int a) {
    a = 0;
    int b = 10;

    a + b; // Trivially dead code

    return 0;
}
