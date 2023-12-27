#include "includes.h"
#include "linkedlist.h"
#include "cpu.h"
#include "memory.h"
#include "disk.h"
#include "curl.h"

void sighandler(int);

int main(int argc, char **argv)
{
  signal(SIGINT, sighandler);

  if (3 != argc)
  {
    printf("Invalid arguments, expecting 2 and recieved %d. Format should be ./app URL TIMEOUT\n", argc - 1);
    exit(1);
  }

  char *url = argv[1];
  int interval = atoi(argv[2]);

  if (0 >= interval)
  {
    printf("Invalid interval\n");
    exit(1);
  }

  for (;;)
  {
    struct Data *requestData = malloc(sizeof(struct Data));
    FILE *stream = open_memstream(&requestData->data, &requestData->size);

    char *hostname = getHostname();
    buildHostnamePostData(stream, hostname);
    fprintf(stream, "&");

    LinkedList *diskStatus = getDiskStatus();
    buildDiskStatusPostData(stream, diskStatus);

    fprintf(stream, "&");

    MemoryStatus *memoryStatus = getMemoryStatus();
    buildMemoryStatusPostData(stream, memoryStatus);

    fprintf(stream, "&");

    LinkedList *cpuStatus = getCpuStatus();
    buildCpuStatusPostData(stream, cpuStatus);

    fclose(stream);
    makePostRequest(url, requestData);
    usleep(interval);
  }

  return 0;
}

void sighandler(int signum)
{
  printf("Caught signal %d, coming out...\n", signum);
  exit(0);
}
