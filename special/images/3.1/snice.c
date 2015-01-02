#include <stdio.h>
#include <unistd.h>
#include <time.h>

void printTime() {
	time_t timep;
	struct tm *p_tm;
	timep = time(NULL);
	p_tm = localtime(&timep);
	printf("%d:%d:%d\n", p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
}

int main(int argc, char *argv[])
{
	int i;
	while (1) {
		printf("%s ", argv[1]);
		printTime();
		for (i = 0; i < 50000000; i++);	
	}

	return 0;
}