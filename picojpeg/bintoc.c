
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

FILE *fpin, *fpout;

uint32_t dlen;
uint8_t data[100000];

int32_t main(void)
{
    uint32_t ra;

    fpin = fopen("small2.jpg", "rb");
    if (fpin == NULL)
    {
        return (1);
    }
    dlen = fread(data, 1, sizeof(data), fpin);
    fclose(fpin);
    printf("%u bytes read\n", dlen);
    fpout = fopen("g_jpegdata.h", "wt");
    if (fpout == NULL)
    {
        return (1);
    }
    fprintf(fpout, "\n");
    fprintf(fpout, "\n");
    fprintf(fpout, "const uint8_t jpegdata[]=\n");
    fprintf(fpout, "{\n");
    for (ra = 0; ra < dlen; ra++)
    {
        fprintf(fpout, "0x%02X,\n", data[ra]);
    }
    fprintf(fpout, "};\n");
    fprintf(fpout, "\n");
    printf("Done.\n");
    return (0);
}
