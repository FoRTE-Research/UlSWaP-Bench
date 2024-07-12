#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

uint8_t data[64384000];

int main(int argc, char *argv[])
{
    unsigned int ra;
    unsigned int rb;

    if (argc < 3)
    {
        printf("bintoh infile outfile\n");
        return (1);
    }
    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        printf("Error opening file [%s]\n", argv[1]);
        return (1);
    }
    rb = fread(data, 1, sizeof(data), fp);
    fclose(fp);
    fp = fopen(argv[2], "wt");
    if (fp == NULL)
    {
        printf("Error creating [%s]\n", argv[2]);
        return (1);
    }
    fprintf(fp, "#include <stdint.h>\n\n");
    fprintf(fp, "uint8_t test_data[] =\n");
    fprintf(fp, "{\n    ");
    for (ra = 0; ra < rb; ra++)
    {
        fprintf(fp, "0x%02X,%s", data[ra], (ra & 7) == 7 ? "\n    " : " ");
    }
    fprintf(fp, "\n");
    fprintf(fp, "};\n");
    fprintf(fp, "\n");
    fprintf(fp, "uint8_t* fakeFile = test_data;\n");
    fclose(fp);

    return (0);
}
