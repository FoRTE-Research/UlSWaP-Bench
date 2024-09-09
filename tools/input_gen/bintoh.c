/*
Takes a binary or text file as input and creates a C header file with the data as an array of bytes from the input file.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

uint8_t data[256000];

void get_macro_from_filename(char* filename, char* macro)
{
    char* dot = strrchr(filename, '.');
    char* slash = strrchr(filename, '/');
    char* start = slash == NULL ? filename : slash + 1;
    char* end = dot == NULL ? filename + strlen(filename) : dot;

    char* p = macro;
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
        printf("Usage: ./bintoh infile outfile\n");
        return (1);
    }

    FILE* pInfile = fopen(argv[1], "rb");
    if (pInfile == NULL)
    {
        printf("Error opening file [%s]\n", argv[1]);
        return 1;
    }
    size_t bytesRead = fread(data, 1, sizeof(data), pInfile);
    fclose(pInfile);

    FILE* pOutFile = fopen(argv[2], "wt");
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
    fprintf(pOutFile, "const uint8_t test_data[] =\n");
    fprintf(pOutFile, "{\n    ");
    for (uint32_t i = 0; i < bytesRead; i++)
    {
        fprintf(pOutFile, "0x%02X,%s", data[i], (i & 7) == 7 ? "\n    " : " ");
        // fprintf(pOutFile, "%02X\n", data[i]);
    }
    fprintf(pOutFile, "\n");
    fprintf(pOutFile, "};\n");
    fprintf(pOutFile, "\n");
    fprintf(pOutFile, "const uint8_t* fakeFile = test_data;\n\n");
    fprintf(pOutFile, "#endif  // %s\n", macro);
    fclose(pOutFile);

    return 0;
}
