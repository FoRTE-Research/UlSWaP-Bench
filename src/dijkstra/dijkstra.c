#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "common.h"

#define NONE 255

typedef struct
{
    uint16_t iDist;
    uint16_t iPrev;
} NODE;

typedef struct _QITEM
{
    struct _QITEM *qNext;
    uint16_t iNode;
    uint16_t iDist;
    uint16_t iPrev;
} QITEM;

/*
 * Dijkstra's algorithm calculates the shortest path between two nodes
 * by calculating the shortest path from 1 node to all the other nodes.
 * At the Nth step, the algorithm "visits" the closest node to
 * any of the N already visited nodes.
 * Then, the algorithm checks the (NUM_NODES - N) unvisited nodes and
 * adds a node to the queue if a shorter path to that node is found.
 * Thus, the maximum number of nodes that could be added to the queue is
 * NUM_NODES + (NUM_NODES - 1) + ... + 2 + 1 ~= (NUM_NODES * NUM_NODES / 2)
 */
#define ARRAY_SIZE (NUM_NODES * NUM_NODES / 2)
QITEM allocated[ARRAY_SIZE];
QITEM *qHead = NULL;
uint16_t notAll = 0;

uint16_t g_qCount = 0;
NODE rgnNodes[NUM_NODES];
uint16_t ch;
uint16_t iPrev, iNode;
uint16_t i, iCost, iDist;

void print_path(NODE *rgnNodes, uint16_t chNode)
{
    if (rgnNodes[chNode].iPrev != NONE)
    {
        print_path(rgnNodes, rgnNodes[chNode].iPrev);
    }
    printf(" %d", chNode);
}

void enqueue(uint16_t iNode, uint16_t iDist, uint16_t iPrev)
{
    QITEM *qNew = &allocated[notAll];
    notAll++;
    QITEM *qLast = qHead;

    if (notAll >= ARRAY_SIZE)
    {
        printf("Out of memory.\r\n");
        exit(1);
    }
    qNew->iNode = iNode;
    qNew->iDist = iDist;
    qNew->iPrev = iPrev;
    qNew->qNext = NULL;

    if (!qLast)
    {
        qHead = qNew;
    }
    else
    {
        qLast = &allocated[notAll - 2];
        qLast->qNext = qNew;
    }
    g_qCount++;
}

void dequeue(uint16_t *piNode, uint16_t *piDist, uint16_t *piPrev)
{
    if (qHead)
    {
        *piNode = qHead->iNode;
        *piDist = qHead->iDist;
        *piPrev = qHead->iPrev;
        qHead = qHead->qNext;
        g_qCount--;
    }
}

uint16_t qcount(void)
{
    return (g_qCount);
}

uint16_t dijkstra(uint16_t chStart, uint16_t chEnd)
{
    notAll = 0;

    for (ch = 0; ch < NUM_NODES; ch++)
    {
        rgnNodes[ch].iDist = NONE;
        rgnNodes[ch].iPrev = NONE;
    }
    if (chStart == chEnd)
    {
        printf("Shortest path is 0 in cost. Just stay where you are.\r\n");
    }
    else
    {
        rgnNodes[chStart].iDist = 0;
        rgnNodes[chStart].iPrev = NONE;
        enqueue(chStart, 0, NONE);

        while (qcount() > 0)
        {
            dequeue(&iNode, &iDist, &iPrev);
            for (i = 0; i < NUM_NODES; i++)
            {
                if ((iCost = AdjMatrix[iNode][i]) != NONE)
                {
                    if ((NONE == rgnNodes[i].iDist) ||
                        (rgnNodes[i].iDist > (iCost + iDist)))
                    {
                        rgnNodes[i].iDist = iDist + iCost;
                        rgnNodes[i].iPrev = iNode;
                        enqueue(i, iDist + iCost, iNode);
                    }
                }
            }
        }

        printf("Shortest path from %2u to %2u is %3u in cost. ", chStart, chEnd, rgnNodes[chEnd].iDist);
        printf("Path is: ");
        print_path(rgnNodes, chEnd);
        printf("\r\n");
    }
    return 0;
}

int benchmark_main(void)
{
    uint16_t i, j;

    /* make a fully connected matrix */
    // see input.h
    /* finds 10 shortest paths between nodes */
    for (i = 0, j = NUM_NODES / 2; i < NUM_NODES; i++, j++)
    {
        j = j % NUM_NODES;
        dijkstra(i, j);
    }
    return 0;
}
