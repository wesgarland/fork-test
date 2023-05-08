#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static int quitSignal;

void signalHandler(int signal)
{
  quitSignal = signal;
}

int main(int argc, const char *argv[])
{
  if (argc < 3) {
    fprintf(stderr, "Expected: %s <evaluator-path> <pass>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  pid_t pid;

  if ((pid = fork()))
  {
    int wstatus;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    printf("forked process %d\n", pid);

    printf("sleeping...\n");
    sleep(3);

    printf("killing...\n");
    if (kill(pid, SIGTERM) == -1) {
      printf("kill failed (%i)\n", errno);
      exit(EXIT_FAILURE);
    }

    printf("waiting...\n");
    (void)wait(&wstatus);
    if (WIFEXITED(wstatus))
      printf("process %d terminated normally with exitCode %i\n", pid, WEXITSTATUS(wstatus));
    else if (WIFSIGNALED(wstatus))
      printf("process %d terminated with default signal handler for signal %i\n", pid, WTERMSIG(wstatus));
    else
      printf("something went really wrong with process %d (%i)\n", pid, wstatus);

    exit(0);
  }

  pid = getpid();
  printf("child process %d begins\n", pid);

  if (atoi(argv[2])) {
    execlp(argv[1], "dcp-evaluator", NULL);
  } else {
    execlp("node", "node", "node_modules/dcp-worker/bin/dcp-evaluator-start", "-e", argv[1], NULL);
  }
}
