/*
 * A C program that does not terminate when SIGINT is generated.
 * Normally, SIGINT is generated with ctrl-c is pressed.
 */
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * This program can be conditionally compiled to install the signal
 * handler with signal() or sigaction(). The distinction is that
 * using signal() only stays within the confines of the Standard
 * C Library and does not assume any POSIX APIs or behavior.
 */

/* #define USE_SIGNAL */
#define USE_SIGACTION

/* Signal Handler for SIGINT */
void sigintHandler(int signo)
{
  if (signo != SIGINT) {
    fprintf(
      stderr,
      "signalHandler: signal != SIGINT (%d != %d)\n",
      signo,
      SIGINT
    );
    exit(1);
  }
#ifdef USE_SIGNAL
  /* Reset handler to catch SIGINT next time.
   *  Refer http://en.cppreference.com/w/c/program/signal
   */
  signal(SIGINT, sigintHandler);
#endif

  printf("\n Cannot be terminated using Ctrl+C \n");
  fflush(stdout);
}

int main(int argc, char **argv)
{
  /*
   * POSIX does not appear to define the default signal mask. It discusses
   * a forked process and created thread inheriting a mask from the creating
   * process/thread, but not the default initial signal mask.
   *
   * On RTEMS 4.11 and older, all signals are masked by default for both POSIX
   *   threads and Classic API tasks. With version 5, they are enabled by
   *   default for POSIX threads and disabled by default for Classic API tasks.
   * On Linux, all signals are unmasked by default.
   *
   * The following code unmasks the signal of interest.
   */
#if __rtems__ && (__RTEMS_MAJOR__ < 5)
  sigset_t old_set,new_set;
  sigprocmask(SIG_SETMASK, NULL, &old_set);
  /* printf( "mask=0x%08lx\n", old_set); */

  sigemptyset(&new_set);
  sigaddset(&new_set,SIGINT);
  sigprocmask(SIG_UNBLOCK, &new_set, NULL);
#endif

#ifdef USE_SIGNAL
  /* Set the SIGINT (Ctrl-C) signal handler to sigintHandler
     Refer http://en.cppreference.com/w/c/program/signal */
  signal(SIGINT, sigintHandler);
#endif

#ifdef USE_SIGACTION
  struct sigaction new_action;
  int              rc;

  new_action.sa_handler = sigintHandler;
  new_action.sa_flags = 0;
  rc = sigaction(SIGINT, &new_action, NULL);
  if (rc != 0) {
    fprintf(stderr, "sigaction -> %s\n", strerror(errno));
  }
#endif

  int sent_signal = 0;

  while(1)
  {
    if (sent_signal == 0) {
      kill(getpid(), SIGINT);
      sent_signal = 1;
    }
  }
  return 0;
}

