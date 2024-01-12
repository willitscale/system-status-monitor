#pragma once

#include "includes.h"

#define BUF_SIZE 2048

typedef struct packet
{
  char *data;
  size_t size;
} Data;

int createSocket(char *, char *);
int sendData(int, Data *);

int createSocket(char *host, char *port)
{
  struct addrinfo hints;
  struct addrinfo *result, *resultPointer;
  int socketFileDescriptor, socketRes;
  size_t len;
  ssize_t nread;
  char buf[BUF_SIZE];

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  socketRes = getaddrinfo(host, port, &hints, &result);
  if (socketRes != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(socketRes));
    exit(EXIT_FAILURE);
  }

  for (resultPointer = result; resultPointer != NULL; resultPointer = resultPointer->ai_next)
  {
    socketFileDescriptor = socket(
        resultPointer->ai_family,
        resultPointer->ai_socktype,
        resultPointer->ai_protocol);

    if (socketFileDescriptor == -1)
    {
      continue;
    }

    if (connect(socketFileDescriptor, resultPointer->ai_addr, resultPointer->ai_addrlen) != -1)
    {
      break;
    }

    close(socketFileDescriptor);
  }

  if (resultPointer == NULL)
  {
    fprintf(stderr, "Could not connect\n");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(result);

  return socketFileDescriptor;
}

int sendData(int socketFileDescriptor, Data *data)
{
  if (BUF_SIZE < data->size + 1 )
  {
    fprintf(stderr, "Message is too long\n");
    return EXIT_FAILURE;
  }

  errno = 0;

  int bytesWritten = write(socketFileDescriptor, data->data, data->size);

  if (0 != errno) {
    fprintf(stderr, "Error: %s (%d)\n", strerror(errno), errno);
    return EXIT_FAILURE;
  }

  // write can return with -1 from time to time due to the outgoing network buffer being full.
  // The following can be used to drain that buffer if the situation requires it, but I'll ommit it for now.
  // if (-1 == bytesWritten) {
  //   fprintf(stderr, "Error: EWOULDBLOCK(-1)\n");
  //   struct pollfd  *pfds;
  //   pfds = calloc(1, sizeof(struct pollfd));
  //   pfds->fd = socketFileDescriptor;
  //   pfds->events = POLLOUT;
  //   poll(pfds, 1, -1);
  //   free(pfds);
  //   return EXIT_FAILURE;
  // }

  if (bytesWritten != data->size)
  {
    fprintf(stderr, "Failed to write message\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}