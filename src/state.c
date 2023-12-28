#include "includes.h"
#include "linkedlist.h"
#include "cpu.h"
#include "memory.h"
#include "disk.h"
#include "udp.h"

void sighandler(int);

int main(int argc, char **argv)
{
  signal(SIGINT, sighandler);

  if (4 != argc)
  {
    printf("Invalid arguments, expecting 3 and recieved %d. Format should be ./app HOST PORT INTERVAL\n", argc - 1);
    exit(1);
  }

  char *host = argv[1];
  char *port = argv[2];
  int interval = atoi(argv[3]);

  if (0 >= interval)
  {
    printf("Invalid interval\n");
    exit(1);
  }

  int socketFileDescriptor = createSocket(host, port);

  for (;;)
  {
    Data *requestData = malloc(sizeof(Data));
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

    sendData(socketFileDescriptor, requestData);

    free(requestData->data);
    free(requestData);

    sleep(interval);
  }

  return 0;
}

void sighandler(int signum)
{
  printf("Caught signal %d, coming out...\n", signum);
  exit(0);
}
