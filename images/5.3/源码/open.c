#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define EVENTOPEN 320

int main(int argc, char ** argv)
{
	int i;

	if (argc != 2)
	{
		return -1;
	}

	i = syscall(EVENTOPEN, atoi(argv[1]));
	printf("open : %d\n", i);
	return 1;
}