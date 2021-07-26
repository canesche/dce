int foo(int a, int b) {
	a = 0;
	b = ++a;
	if (a != b) {
		a = 10;
		b++;
	}
	return a + b;
}
