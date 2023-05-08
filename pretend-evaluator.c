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
  static const char msg[] = "trapped signal ";
  static char buf[3];

  quitSignal = signal;
  write(STDOUT_FILENO, msg, sizeof(msg) -1);
  buf[0] = '0' + signal / 10;
  buf[1] = '0' + signal % 10;
  buf[2] = '\n';
  write(STDOUT_FILENO, buf, sizeof(buf));
}

int main(int argc, const char *argv[])
{
  pid_t pid = getpid();
  printf("pretend evaluator %d begins\n", pid);
  signal(SIGTERM, signalHandler);
  signal(SIGINT, signalHandler);

  while(!quitSignal)
    sleep(1);

  printf("pretend evaluator %d quitting due trapped signal %i\n", pid, quitSignal);
  exit(100 + quitSignal);
}

