int main() {
	int a = 100;
	int b = 200;
	int c = a-b;
	int *g = &a;
	b = c + *g;
	c = b * a;
	return 0;
}
