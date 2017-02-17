/*
Author		: Sahil Khan
Date		: 16th Feb 2017
Description	: This lab covers the concepts of how you can come out of a signal hanlder 
		  by calling siglongjmp() or longjmp() functions. The signal mask status before
		  and after going into handler is printed so as to get an idea how the signal
		  mask gets updated when siganl is handled. Kernel puts the signal in the current
		  process sigMask so that same signal cannot intterrupt its own signal handler.
		  
		  However in this program there is a problem, when control comes out of signal 
		  handler using lngjmp() function the kernel has no idea to update sigMask for
		  this process , so even if the control is back to main() function, the signal 
		  sill remains in the sigMask of this process. Due to this further Ctrl+C signal
		  is not handled as expected.
		  
		  If the control comes out of signal handler normally using a return statememt 
		  then only Kernel updates sigMask for this process and further Ctrl+C signal 
		  can be handled normally.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <setjmp.h>

static volatile sig_atomic_t canJump = 0;

#ifdef USE_SIGSETJMP
	static sigjmp_buf senv;
#else
	static jmp_buf env;
#endif


void                    /* Print list of signals within a signal set */
printSigset(FILE *of, const char *prefix, const sigset_t *sigset)
{
    int sig, cnt;

    cnt = 0;
    for (sig = 1; sig < NSIG; sig++) {
        if (sigismember(sigset, sig)) {
            cnt++;
            fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
        }
    }

    if (cnt == 0)
        fprintf(of, "%s<empty signal set>\n", prefix);
}

int                     /* Print mask of blocked signals for this process */
printSigMask(FILE *of, const char *msg)
{
    sigset_t currMask;

    if (msg != NULL)
        fprintf(of, "%s", msg);

    if (sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
        return -1;

    printSigset(of, "\t\t", &currMask);

    return 0;
}

int                     /* Print signals currently pending for this process */
printPendingSigs(FILE *of, const char *msg)
{
    sigset_t pendingSigs;

    if (msg != NULL)
        fprintf(of, "%s", msg);

    if (sigpending(&pendingSigs) == -1)
        return -1;

    printSigset(of, "\t\t", &pendingSigs);

    return 0;
}

static void sig_handler(int sig_num)
{	
	printf("Received signal %d (%s), signal mask is:\n",sig_num,strsignal(sig_num));
	printSigMask(stdout,NULL);
	
	if(!canJump){
		printf("'env' buffer not yet set, doing a simple return\n");
		return;
	}
	
#ifdef USE_SIGSETJMP
	siglongjmp(senv,1);
#else
	longjmp(env,1);
#endif
}

int main(int argc, char *argv[])
{
	struct sigaction sa;

    printSigMask(stdout, "Signal mask at startup:\n");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        //errExit("sigaction");

#ifdef USE_SIGSETJMP
    printf("Calling sigsetjmp()\n");
    if (sigsetjmp(senv, 1) == 0)
#else
    printf("Calling setjmp()\n");
    if (setjmp(env) == 0)
#endif
        canJump = 1;                    /* Executed after [sig]setjmp() */

    else                                /* Executed after [sig]longjmp() */
        printSigMask(stdout, "After jump from handler, signal mask is:\n" );

    for (;;)                            /* Wait for signals until killed */
        pause();
}
