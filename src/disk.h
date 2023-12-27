#pragma once

#include "includes.h"
#include "linkedlist.h"
#include "system.h"

#define DISK_DEVICE 0
#define DISK_BLOCKS 1
#define DISK_USED 2
#define DISK_AVAILABLE 3
#define DISK_USE 4
#define DISK_MOUNT 5

typedef struct disk
{
    char *device;
    long blocks;
    long used;
    long available;
    long use;
    char *mount;
} DiskStatus;

LinkedList *getDiskStatus();
void freeDiskStatus(DiskStatus *);
void printDiskStatus();
void buildDiskStatusPostData(FILE *, LinkedList *);

LinkedList *getDiskStatus()
{
    LinkedList *ll = execute("df -t ext4");
    LinkedList *llPtr = ll;

    LinkedList *linkedList = initLinkedList();
    LinkedList *linkedListPtr = linkedList;

    int rows = 0;
    while (NULL != ll->data)
    {
        if (0 == rows++)
        {
            ll = ll->next;
            continue;
        }
        int offset = 0;
        char *token = strtok(ll->data, " ");
        DiskStatus *diskStatus = calloc(1, sizeof(DiskStatus));
        while (NULL != token)
        {
            switch (offset++)
            {
            case DISK_DEVICE:
                diskStatus->device = atos(token);
                break;
            case DISK_BLOCKS:
                diskStatus->blocks = atoi(token);
                break;
            case DISK_USED:
                diskStatus->used = atoi(token);
                break;
            case DISK_AVAILABLE:
                diskStatus->available = atoi(token);
                break;
            case DISK_USE:
                diskStatus->use = atoi(token);
                break;
            case DISK_MOUNT:
                diskStatus->mount = atos(token);
                break;
            }
            token = strtok(NULL, " ");
        }
        linkedList = addToLinkedList(linkedList, diskStatus);
        ll = ll->next;
    }
    freeLinkedList(llPtr);
    return linkedListPtr;
}

void printDiskStatus()
{
    LinkedList *ll = getDiskStatus();
    LinkedList *llPtr = ll;
    while (NULL != ll->data)
    {
        DiskStatus *diskStatus = (DiskStatus *)ll->data;
        printf("%s use %lu\n", diskStatus->device, diskStatus->used);
        printf("%s size %lu\n", diskStatus->device, diskStatus->blocks);
        freeDiskStatus(ll->data);
        ll = ll->next;
    }
    freeLinkedList(llPtr);
}

void freeDiskStatus(DiskStatus *diskStatus)
{
    free(diskStatus->device);
    free(diskStatus->mount);
}

void buildDiskStatusPostData(FILE *stream, LinkedList *diskStatusList)
{
    LinkedList *diskStatusListHead = diskStatusList;
    while (NULL != diskStatusList->data)
    {
        DiskStatus *diskStatus = (DiskStatus *)diskStatusList->data;
        fprintf(stream, "disk[%s][used]=%ld", diskStatus->device, diskStatus->used);
        fprintf(stream, "&");
        fprintf(stream, "disk[%s][total]=%ld", diskStatus->device, diskStatus->blocks);

        freeDiskStatus(diskStatusList->data);
        diskStatusList = diskStatusList->next;
        if (NULL != diskStatusList->data)
        {
            fprintf(stream, "&");
        }
    }
    freeLinkedList(diskStatusListHead);
}
