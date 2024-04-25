#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "input_large.h"

#define UNLIMIT

int32_t compare(const void *elem1, const void *elem2)
{
    /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
    /* sort based on distances from the origin... */

    double distance1, distance2;

    distance1 = (*((struct my3DVertexStruct *)elem1)).distance;
    distance2 = (*((struct my3DVertexStruct *)elem2)).distance;

    return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}

int benchmark_main(void)
{
    uint32_t i, count = 0;

    for (count = 0; count < sizeof(array) / sizeof(struct my3DVertexStruct); ++count)
    {
        array[count].distance = sqrt(pow(array[count].x, 2) + pow(array[count].y, 2) + pow(array[count].z, 2));
    }

    printf("Sorting %u vectors based on distance from the origin.\r\n\r\n", count);
    qsort(array, count, sizeof(struct my3DVertexStruct), compare);

    for (i = 0; i < count; i++)
    {
        printf("%d %d %d\r\n", array[i].x, array[i].y, array[i].z);
    }
    return 0;
}
