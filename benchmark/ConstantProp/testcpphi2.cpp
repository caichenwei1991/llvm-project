int main() {
	int a = 1;
	int b = 2;
	int c = 3;
	a = b + c;
	if (b < c) {
		b = b + 2;
		a = b + 3;
		c = b + 4;
	}
	else {
		b = b + 5;
		a = b + 6;
		c = b + 7;
	}
	a = 5;
	if (a == b) {
		a = a + 1;
		b = b + 2;
	}
	else {
		a = a + 2;
		b = b + 3;
	}
	return 0;
}
