#pragma once

#include "includes.h"
#include "linkedlist.h"
#include "system.h"
#include "library.h"

#define CPU_NAME 0
#define CPU_USER 1
#define CPU_NICE 2
#define CPU_SYSTEM 3
#define CPU_IDLE 4
#define CPU_IOWAIT 5
#define CPU_IRQ 6
#define CPU_SOFTIRQ 7
#define CPU_STEAL 8
#define CPU_GUEST 9
#define CPU_GUEST_NICE 10

typedef struct cpu
{
    char *name;
    int user;       // (1) Time spent in user mode.
    int nice;       // (2) Time spent in user mode with low priority (nice).
    int system;     // (3) Time spent in system mode.
    int idle;       // (4) Time spent in the idle task.  This value should be USER_HZ times the second entry in the /proc/uptime pseudo-file.
    int iowait;     // (5) Time waiting for I/O to complete.
    int irq;        // (6) Time servicing interrupts.
    int softirq;    // (7) Time servicing softirqs.
    int steal;      // (8) Stolen time, which is the time spent in other operating systems when running in a virtualized environment
    int guest;      // (9) Time spent running a virtual CPU for guest operating systems under the control of the Linux kernel.
    int guest_nice; // (10) Time spent running a niced guest (virtual CPU for guest operating systems under the control of the Linux kernel).
} CpuStatus;

LinkedList *getCpuStatus();
void freeCpuStatus(CpuStatus *);
void printCpuStatus();
void buildCpuStatusPostData(FILE *, LinkedList *);

LinkedList *getCpuStatus()
{
    LinkedList *processOutput = execute("grep -E 'cpu[0-9]+' /proc/stat");
    LinkedList *processOutputHead = processOutput;

    LinkedList *linkedList = initLinkedList();
    LinkedList *linkedListPtr = linkedList;

    while (NULL != processOutput->data)
    {
        int offset = 0;
        char *token = strtok(processOutput->data, " ");
        CpuStatus *cpuStatus = calloc(1, sizeof(CpuStatus));
        while (NULL != token)
        {
            switch (offset++)
            {
            case CPU_NAME:
                cpuStatus->name = atos(token);
                break;
            case CPU_USER:
                cpuStatus->user = atoi(token);
                break;
            case CPU_NICE:
                cpuStatus->nice = atoi(token);
                break;
            case CPU_SYSTEM:
                cpuStatus->system = atoi(token);
                break;
            case CPU_IDLE:
                cpuStatus->idle = atoi(token);
                break;
            case CPU_IOWAIT:
                cpuStatus->iowait = atoi(token);
                break;
            case CPU_IRQ:
                cpuStatus->irq = atoi(token);
                break;
            case CPU_SOFTIRQ:
                cpuStatus->softirq = atoi(token);
                break;
            case CPU_STEAL:
                cpuStatus->steal = atoi(token);
                break;
            case CPU_GUEST:
                cpuStatus->guest = atoi(token);
                break;
            case CPU_GUEST_NICE:
                cpuStatus->guest_nice = atoi(token);
                break;
            }
            token = strtok(NULL, " ");
        }
        linkedList = addToLinkedList(linkedList, cpuStatus);
        processOutput = processOutput->next;
    }
    freeLinkedList(processOutputHead);
    return linkedListPtr;
}

void freeCpuStatus(CpuStatus *cpuStatus)
{
    free(cpuStatus->name);
}

void printCpuStatus()
{
    LinkedList *cpuStatusList = getCpuStatus();
    LinkedList *cpuStatusListHead = cpuStatusList;

    while (NULL != cpuStatusList->data)
    {
        CpuStatus *cpuStatus = (CpuStatus *)cpuStatusList->data;
        int cpuActive = cpuStatus->user + cpuStatus->nice + cpuStatus->system + cpuStatus->irq +
                        cpuStatus->softirq + cpuStatus->steal + cpuStatus->guest + cpuStatus->guest_nice;
        int cpuTotal = cpuStatus->idle + cpuStatus->iowait + cpuActive;
        float cpuUsage = ((float)cpuActive / (float)cpuTotal) * 100;
        printf("%s: %d/%d %0.2f%%\n", cpuStatus->name, cpuActive, cpuTotal, cpuUsage);
        freeCpuStatus(cpuStatusList->data);
        cpuStatusList = cpuStatusList->next;
    }

    freeLinkedList(cpuStatusListHead);
}

void buildCpuStatusPostData(FILE *stream, LinkedList *cpuStatusList)
{
    LinkedList *cpuStatusListHead = cpuStatusList;

    while (NULL != cpuStatusList->data)
    {
        CpuStatus *cpuStatus = (CpuStatus *)cpuStatusList->data;

        int cpuActive = cpuStatus->user + cpuStatus->nice + cpuStatus->system +
                        cpuStatus->irq + cpuStatus->softirq + cpuStatus->steal +
                        cpuStatus->guest + cpuStatus->guest_nice;

        int cpuTotal = cpuStatus->idle + cpuStatus->iowait + cpuActive;

        fprintf(stream, "cpu[%s][active]=%d", cpuStatus->name, cpuActive);
        fprintf(stream, "&");
        fprintf(stream, "cpu[%s][total]=%d", cpuStatus->name, cpuTotal);

        freeCpuStatus(cpuStatusList->data);
        cpuStatusList = cpuStatusList->next;
        if (NULL != cpuStatusList->data)
        {
            fprintf(stream, "&");
        }
    }

    freeLinkedList(cpuStatusListHead);
}
