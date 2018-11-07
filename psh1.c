/* prompting shell version 1
 *      prompts for the command and its arguments.
 *      builds the argument vector for the call to execvp
 *      uses execvp(), and nver returns
 */

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define MAXARGS 20     /* cmdline args */
#define ARGLEN 100     /* token lenght */

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
	execvp(arglist[0], arglist);  // do it
	perror("execvp failed");
	exit(1);
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
