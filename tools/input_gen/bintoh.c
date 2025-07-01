/*
Takes a binary or text file as input and creates a C header file with the data as an array of bytes from the input file.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

uint8_t data[256000];

void get_macro_from_filename(char *filename, char *macro)
{
    char *dot = strrchr(filename, '.');
    char *slash = strrchr(filename, '/');
    char *start = slash == NULL ? filename : slash + 1;
    char *end = dot == NULL ? filename + strlen(filename) : dot;

    char *p = macro;
    for (uint32_t i = 0; i < end - start; i++)
    {
        macro[i] = toupper(*(start + i));
        p++;
    }
    *p++ = '_';
    *p++ = 'H';
    *p = 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: ./bintoh infile outfile array_size\n");
        return (1);
    }

    uint32_t array_size = 0;
    if (argc >= 4)
    {
        array_size = atoi(argv[3]);
        if (array_size > sizeof(data))
        {
            printf("Size exceeds maximum of %zu bytes\n", sizeof(data));
            return 1;
        }
    }

    int32_t constant = 0;
    if (argc == 5 && strcmp(argv[4], "-c") == 0)
    {
        constant = 1;
    }

    FILE *pInfile = fopen(argv[1], "rb");
    if (pInfile == NULL)
    {
        printf("Error opening file [%s]\n", argv[1]);
        return 1;
    }
    size_t bytesRead = fread(data, 1, sizeof(data), pInfile);
    array_size = array_size < bytesRead ? array_size : bytesRead;
    fclose(pInfile);

    FILE *pOutFile = fopen(argv[2], "wt");
    if (pOutFile == NULL)
    {
        printf("Error creating [%s]\n", argv[2]);
        return 1;
    }
    char macro[256];
    get_macro_from_filename(argv[2], macro);

    fprintf(pOutFile, "#ifndef %s\n", macro);
    fprintf(pOutFile, "#define %s\n\n", macro);

    fprintf(pOutFile, "#include <stdint.h>\n\n");
    fprintf(pOutFile, "#define INPUT_SIZE %u\n\n", array_size);
    fprintf(pOutFile, "#ifdef INPUT_IMPLEMENTATION\n");

    if (constant)
    {
        fprintf(pOutFile, "const uint8_t test_data[] =\n");
    }
    else
    {
        fprintf(pOutFile, "uint8_t test_data[] =\n");
    }
    fprintf(pOutFile, "{\n    ");

    for (uint32_t i = 0; i < array_size; i++)
    {
        fprintf(pOutFile, "0x%02X,%s", data[i], (i & 7) == 7 ? "\n    " : " ");
    }
    fprintf(pOutFile, "\n");
    fprintf(pOutFile, "};\n");
    fprintf(pOutFile, "\n");
    if (constant)
    {
        fprintf(pOutFile, "const uint8_t* fakeFile = test_data;\n\n");
    }
    else
    {
        fprintf(pOutFile, "uint8_t* fakeFile = test_data;\n\n");
    }

    fprintf(pOutFile, "#endif  // INPUT_IMPLEMENTATION\n");
    fprintf(pOutFile, "#endif  // %s\n", macro);
    fclose(pOutFile);

    return ret;
}
