#include "includes.h"
#include "linkedlist.h"
#include "cpu.h"
#include "memory.h"
#include "disk.h"
#include "udp.h"

#define EXPECTED_NUMBER_OF_ARGUMENTS 4

void sighandler(int);

int main(int argc, char **argv)
{
  signal(SIGINT, sighandler);

  if (EXPECTED_NUMBER_OF_ARGUMENTS != argc)
  {
    printf("Invalid arguments, expecting 3 and recieved %d. Format should be ./app HOST PORT INTERVAL\n", argc - 1);
    exit(EXIT_FAILURE);
  }

  char *host = argv[1];
  char *port = argv[2];
  int interval = atoi(argv[3]);

  if (0 >= interval)
  {
    printf("Invalid interval of %s\n", argv[3]);
    exit(EXIT_FAILURE);
  }

  int socketFileDescriptor = createSocket(host, port);
  char *hostname = getHostname();

  for (;;)
  {
    Data *requestData = malloc(sizeof(Data));
    FILE *stream = open_memstream(&requestData->data, &requestData->size);

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
  free(hostname);

  return EXIT_SUCCESS;
}

void sighandler(int signum)
{
  printf("Caught signal %d, coming out...\n", signum);
  exit(EXIT_SUCCESS);
}
