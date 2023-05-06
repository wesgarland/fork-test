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
  pid_t pid;

  signal(SIGINT, signalHandler);
  
  if ((pid = fork()))
  {
    int wstatus;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    printf("forked process %d\n", pid);
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

  while(!quitSignal)
    sleep(1);

  printf("child process %d trapped signal %i\n", pid, quitSignal);
  exit(100 + quitSignal);
}

