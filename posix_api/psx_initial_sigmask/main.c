/*
 * A C program that displays the default process signal mask and ensures
 * that a created thread inherits its initial signal mask from the thread
 * that created it.
 */

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sigset_t main_sigmask;

/*
 * On Linux, sigset_t is a structure with an array in it. The size of
 * the entire structure can be larger than the array itself due to padding.
 * This is intended to ensure we only print and compare valid contents.
 *
 * Note if Linux moves beyond 1024 bits in the structure, this will break.
 */
#ifdef __linux__
  #define SIGSET_NBYTES (_SIGSET_NWORDS * sizeof(unsigned long int))
  #define SIGSET_NWORDS _SIGSET_NWORDS
#else
  #define SIGSET_NBYTES (sizeof(sigset_t))
  #define SIGSET_NWORDS (sizeof(sigset_t) / sizeof(unsigned long int))
#endif

static void print_sigmask(const char *name, sigset_t *sigset_p)
{
  int            rc;
  unsigned char *p;
  int            i;

  rc = sigemptyset(sigset_p);
  assert(rc == 0);

  rc = sigprocmask(SIG_SETMASK, NULL, sigset_p);
  assert(rc == 0);
  printf("%s signal mask (in hex):\n    ", name);

  /*
   * There is no assurance that sigset_t is a primitive type so
   * we have to print it a long at a time.
   */
  p = (unsigned char *) sigset_p;
  for (i=0 ; i < SIGSET_NBYTES ; i++) {
    printf("%02x%s", *p++, (i % 16 == 15) ? "\n    " : " ");
  }
  printf("\n");
}

static void block_sigmask(int signo, sigset_t *sigset_p)
{
  sigset_t sigset;
  int      rc;

  /*
   * Block the requested signal
   */
  rc = sigemptyset(&sigset);
  assert(rc == 0);
  rc = sigaddset(&sigset, signo);
  assert(rc == 0);

  rc = sigprocmask(SIG_BLOCK, &sigset, NULL);
  assert(rc == 0);

  /*
   * Fetch the current signal mask reflecting the requested update
   */
  sigemptyset(sigset_p);
  rc = sigprocmask(SIG_SETMASK, NULL, sigset_p);
  assert(rc == 0);
}

static void *thread_body(void *arg)
{
  sigset_t    mask;
  const char *name = (const char *)arg;

  /*
   * There is no assurance that sigset_t is a primitive type so
   * we have to use memcmp().
   */
  printf("Ensure %s mask equals parent's mask\n", name);
  print_sigmask("main", &main_sigmask);
  print_sigmask(arg, &mask);
  assert(memcmp(&main_sigmask, &mask, SIGSET_NWORDS) == 0);
  
  return NULL;
}

int main(int argc, char **argv)
{
  int        rc;
  pthread_t  id;

  puts("*** START OF SIGNAL MASK TEST ***");

  sigset_t empty;
  print_sigmask("empty", &empty);

  print_sigmask("main", &main_sigmask);

  rc = pthread_create(&id, NULL, thread_body, "thread1");
  assert(rc == 0);
  sleep(1);

  block_sigmask(SIGUSR1, &main_sigmask);
  rc = pthread_create(&id, NULL, thread_body, "thread2");
  assert(rc == 0);
  sleep(1);
  

  puts("*** END OF SIGNAL MASK TEST ***");
  exit(0);
  return 0;
}

