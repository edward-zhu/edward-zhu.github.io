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
	int status;
	int insyscall = 0;
	unsigned long i, length, j;
	unsigned long begin;
	char * str = NULL, * pos = NULL;
	WORD w;

	if (child == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl("/bin/ls", "ls", NULL);
	}
	else {
		while(1) {
			wait(&status); // wait for child process make a syscall.
			if(WIFEXITED(status)) {
				break;
			}
			ptrace(PTRACE_GETREGS, child, NULL, &regs);
			if (insyscall == 0) {
				insyscall = 1;
				printf("syscall: no.%llu params %llu, %llu, %llu\n", regs.orig_rax, regs.rdi, regs.rsi, regs.rdx);
				if (regs.orig_rax == SYS_write) {
					printf("------>write syscall\n");
					length = regs.rdx;
					begin = regs.rsi;
					str = (char *)malloc(sizeof(char) * (length + 1));
					pos = str;
					for (i = 0; i < length / WORD_SIZE; i++) {
						w.val = ptrace(PTRACE_PEEKDATA, child, begin + i * WORD_SIZE, NULL);
						memcpy(pos, w.chars, WORD_SIZE);
						pos += WORD_SIZE;
					}
					j = length % WORD_SIZE;
					if (j) {
						w.val = ptrace(PTRACE_PEEKDATA, child, begin + i * WORD_SIZE, NULL);
						memcpy(pos, w.chars, j);
					}
					str[length] = '\0';
					printf("\n---------->write : \n%s \n\n", str);
				}
			}
			else {
				printf("syscall: returns %lld\n", regs.rax);
				insyscall = 0;
			}
			
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}
		
	}

	return 0;
}