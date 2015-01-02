#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define EVENTCLOSE	321
#define EVENTWAIT	322
#define EVENTSIG	323
#define EVENTSHOW	324

int main(int argc, char ** argv)
{
	syscall(EVENTSHOW);
	return 1;
}