#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/user.h>

#define WORD_SIZE (sizeof(long))

union word {
	unsigned long long int val;
	char chars[WORD_SIZE];
};

typedef union word WORD;

int main()
{
	pid_t child;
	struct user_regs_struct regs;
	child = fork();

	if (child == 0) {
		execl("/bin/ls", "ls", NULL);
	}
	else {
		printf("child pid : %u\n", child);
		wait(NULL);
		printf("child process returned.\n");
		
	}

	return 0;
}