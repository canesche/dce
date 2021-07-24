void foo(int a, int b) {
    a = 0;
    b = 50;
    while (a < 52) {
        a++;
    }
    while (b > 10) {
        b--;
        if (a == b) { // codigo morto
            printf("loooll\n");
            break;
        }
    }
}