#include <stdio.h>

int func(int n)
{
	if (n < 1) {
		return 0;
	}
	if (n == 1 || n == 2) {
		return 1;
	}
	return func(n - 1) + func(n - 2);
}

int main()
{
	printf("%d\n", func(5));

	return;
}