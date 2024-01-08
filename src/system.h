#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "library.h"

LinkedList *execute(char *);
char *getHostname();
void buildHostnamePostData(FILE *, char *);

LinkedList *execute(char *command)
{
  char path[1024];
  FILE *filePointer = popen(command, "r");
  LinkedList *linkedList = initLinkedList();
  LinkedList *linkedListPtr = linkedList;

  if (filePointer == NULL)
  {
    printf("Could not run command: %s\n", command);
    exit(EXIT_FAILURE);
  }

  while (fgets(path, sizeof(path), filePointer) != NULL)
  {
    linkedList = addToLinkedList(linkedList, atos(path));
  }

  pclose(filePointer);

  return linkedListPtr;
}

char *getHostname()
{
  LinkedList *hostnameList = execute("hostname");
  char *hostname = malloc(strlen(hostnameList->data));
  memcpy(hostname, hostnameList->data, strlen(hostnameList->data) - 1);
  freeLinkedList(hostnameList);
  return hostname;
}

void buildHostnamePostData(FILE *stream, char *hostname)
{
  fprintf(stream, "hostname=%s", hostname);
}
