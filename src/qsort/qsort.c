#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "input.h"


int compare(const void *elem1, const void *elem2)
{
    /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
    /* sort based on distances from the origin... */

    float distance1, distance2;

    distance1 = (*((Vertex *)elem1)).distance;
    distance2 = (*((Vertex *)elem2)).distance;

    return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}

int benchmark_main(void)
{
    uint16_t i;
    uint16_t array_size = sizeof(vertices) / sizeof(vertices[0]);

    for (i = 0; i < array_size; ++i)
    {
        float x2 = (int32_t)vertices[i].x * (int32_t)vertices[i].x;
        float y2 = (int32_t)vertices[i].y * (int32_t)vertices[i].y;
        float z2 = (int32_t)vertices[i].z * (int32_t)vertices[i].z;
        vertices[i].distance = sqrtf(x2 + y2 + z2);
    }
    printf("Sorting %" PRIu32 " vectors based on distance from the origin.\r\n\r\n", array_size);
    qsort(vertices, array_size, sizeof(Vertex), compare);

    printf("   x        y        z     =>  distance\r\n");
    printf("---------------------------------------\r\n");
    for (i = 0; i < array_size; i++)
    {
        printf("%7d  %7d  %7d  =>  %8.2f\r\n", vertices[i].x, vertices[i].y, vertices[i].z, printf_float(vertices[i].distance));
    }
    return 0;
}
