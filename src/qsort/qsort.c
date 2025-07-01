#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "input.h"

#define UNLIMIT

int32_t compare(const void *elem1, const void *elem2)
{
    /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
    /* sort based on distances from the origin... */

    float distance1, distance2;

    distance1 = (*((Vertex *)elem1)).distance;
    distance2 = (*((Vertex *)elem2)).distance;

    return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}

uint32_t benchmark_main(void)
{
    uint32_t ret = 0;

    uint16_t i;

    for (i = 0; i < ARRAY_SIZE; ++i)
    {
        vertices[i].distance = sqrt(pow(vertices[i].x, 2) + pow(vertices[i].y, 2) + pow(vertices[i].z, 2));
    }

    printf("Sorting %u vectors based on distance from the origin.\r\n\r\n", ARRAY_SIZE);
    qsort(vertices, ARRAY_SIZE, sizeof(Vertex), compare);

    printf("   x        y        z     =>  distance\r\n");
    printf("---------------------------------------\r\n");
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        printf("%7d  %7d  %7d  =>  %8.2f\r\n", vertices[i].x, vertices[i].y, vertices[i].z, printf_float(vertices[i].distance));
    }
    return ret;
}
