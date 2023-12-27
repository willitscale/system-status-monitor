#pragma once

#include "includes.h"

typedef struct ll
{
  void *data;
  struct ll *next;
} LinkedList;

LinkedList *addToLinkedList(LinkedList *, void *);
LinkedList *initLinkedList();
void freeLinkedList(LinkedList *);

LinkedList *initLinkedList()
{
  return calloc(1, sizeof(LinkedList));
}

LinkedList *addToLinkedList(LinkedList *linkedList, void *data)
{
  LinkedList *next = initLinkedList();
  linkedList->data = data;
  linkedList->next = next;
  return next;
}

void freeLinkedList(LinkedList *linkedList)
{
  while (NULL != linkedList)
  {
    free(linkedList->data);
    LinkedList *tmp = linkedList;
    linkedList = linkedList->next;
    free(tmp);
  }
}
