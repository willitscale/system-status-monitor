#pragma once

#include "includes.h"
#include "linkedlist.h"

#define MEMORY_VALUE 1

typedef struct memory
{
    int total;
    int free;
    int available;
} MemoryStatus;

MemoryStatus *getMemoryStatus();
void printMemoryStatus();

MemoryStatus *getMemoryStatus()
{
    LinkedList *ll = execute("cat /proc/meminfo");
    LinkedList *llPtr = ll;

    MemoryStatus *memoryStatus = calloc(1, sizeof(MemoryStatus));

    while (NULL != ll->data)
    {
        int *valPtr;
        if (NULL != strstr(ll->data, "MemTotal"))
        {
            valPtr = &memoryStatus->total;
        }
        else if (NULL != strstr(ll->data, "MemFree"))
        {
            valPtr = &memoryStatus->free;
        }
        else if (NULL != strstr(ll->data, "MemAvailable"))
        {
            valPtr = &memoryStatus->available;
        }
        else
        {
            ll = ll->next;
            continue;
        }

        int offset = 0;
        char *token = strtok(ll->data, " ");
        while (NULL != token)
        {
            if (MEMORY_VALUE == offset++)
            {
                int intVal = atoi(token);
                memcpy(valPtr, &intVal, sizeof(int));
            }
            token = strtok(NULL, " ");
        }
        ll = ll->next;
    }

    freeLinkedList(llPtr);

    return memoryStatus;
}

void printMemoryStatus()
{
    MemoryStatus *memoryStatus = getMemoryStatus();
    printf(
        "Total: %d\nFree: %d\nAvailable: %d\n",
        memoryStatus->total,
        memoryStatus->free,
        memoryStatus->available);
}

void buildMemoryStatusPostData(FILE *stream, MemoryStatus *memoryStatus)
{
    fprintf(stream, "memory[free]=%d", memoryStatus->free);
    fprintf(stream, "&");
    fprintf(stream, "memory[available]=%d", memoryStatus->available);
    fprintf(stream, "&");
    fprintf(stream, "memory[total]=%d", memoryStatus->total);
    free(memoryStatus);
}
