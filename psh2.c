/** prompting shell version 2 (psh2.c)
 **
 **    Solves the 'one-shot' problem of version 1
 **           Uses execvp(), but fork()s first so that the
 **           shell waits around to perform another command
 **    New problem: shell catches signals. Run vi, press ^c.
 **/

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define MAXARGS 20   /* cmdline args */
#define ARGLEN 100   /* token length */

char* makestring(char*);
int execute(char*[]);

int main()
{
	char *arglist[MAXARGS+1];    /* an array of ptrs */
	int numargs = 0;             /* index into array */
	char argbuf[ARGLEN];         /* read stuff here */

	while (numargs < MAXARGS) {
		printf("Arg[%d]? ", numargs);
		
		/* 입력 첫 문자가 '\n'이 아닐 때 */
		if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
			arglist[numargs++] = makestring(argbuf);
		else {
			/* 입력 첫 문자가 '\n'일 때 */
			if (numargs > 0) {                 // any args?
				arglist[numargs] = NULL;   // close list
				execute(arglist);          // do it
				numargs = 0;               // and reset
			}
		}
	}

	return 0;
}


/* use execvp to do it */
int execute(char *arglist[])
{
	int pid, exitstatus;   /* of child */

	pid = fork();          /* make new process */

	switch (pid) {
		case -1:
			perror("fork failed");
			exit(1);
		case 0:
			execvp(arglist[0], arglist);   /* do it */
			perror("execvp failed");
			exit(1);
		default:
			while (wait(&exitstatus) != pid)
				;
			printf("child exited with status %d, %d\n", exitstatus>>8, exitstatus&0377);
	}
}


/* trim off newline and create storage for the string */
char* makestring(char *buf)
{
	char *cp;

	buf[strlen(buf)-1] = '\0';     // trim newline

	cp = malloc(strlen(buf)+1);    // get memory
	if (cp == NULL) {
		fprintf(stderr, "no memory\n");
		exit(1);
	}

	strcpy(cp, buf);    // copy chars

	return cp;          // return ptr
}
