int foo(int a, int b) {
    
    b = 2;
    a = 1 + b;

    switch (a) {
    case 1:
        printf("Ola mundo\n");
        b = 1;
        break;
    case 2:
        printf("Ola mundo2\n");
        b = 2;
        break;
    case 3:
        printf("Ola mundo3\n");
        b = 3;
        break;
    default:
        printf("Ola mundo 4\n");
        b = 100;
        break;
    }

    return b;
}