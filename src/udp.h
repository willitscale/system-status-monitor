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

  if (write(socketFileDescriptor, data->data, data->size) != data->size)
  {
    fprintf(stderr, "Failed to write message\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}