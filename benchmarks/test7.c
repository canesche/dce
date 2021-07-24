int foo(int a, int b) {
    a = 1;
    b = 1;
    
    if (a != b) { // codigo morto
        return 0;
    }
    return 1;
}