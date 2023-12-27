#pragma once

#include "includes.h"

#define BUF_SIZE 1024

typedef struct packet
{
  char *data;
  size_t size;
} Data;

void sendData(char *, char *, Data *);


void sendData(char * host, char * port, Data * data)
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
        resultPointer->ai_protocol
    );

    if (socketFileDescriptor == -1)
    {
      continue;
    }
    
    if (connect(socketFileDescriptor, resultPointer->ai_addr, resultPointer->ai_addrlen) != -1) {
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

    if (data->size + 1 > BUF_SIZE)
    {
      fprintf(stderr, "Message is too long\n");
      exit(EXIT_FAILURE);
    }

    if (write(socketFileDescriptor, data->data, data->size) != data->size)
    {
      fprintf(stderr, "partial/failed write\n");
      exit(EXIT_FAILURE);
    }
}