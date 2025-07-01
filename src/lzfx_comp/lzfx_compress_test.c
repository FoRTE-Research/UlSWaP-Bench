#include <stdio.h>
#include <stdint.h>
#include "input.h"
#include "lzfx.h"
#include "common.h"

#define OUTPUT_FILENAME "lzfx_compressed_data.h"
#define COMPRESSED_SIZE 5000
uint8_t compressed_data[COMPRESSED_SIZE];

benchmark_hash_t benchmark_main(void)
{
    benchmark_hash_t benchmark_hash_ret = 0;

    uint32_t text_size = sizeof(test_data);
    uint32_t compressed_size = COMPRESSED_SIZE;

    printf("Compressing %u bytes of data...\r\n", text_size);
    int32_t status = lzfx_compress(test_data, text_size, compressed_data, &compressed_size);

    if (status < 0)
    {
        printf("Compression failed with error code %d\r\n", status);
        return 1;
    }

    printf("Compressed size: %u bytes\r\n", compressed_size);

    // compute checksum on compressed data
    uint32_t checksum = 0;
    volatile uint32_t noprint_output;
    for (uint32_t i = 0; i < compressed_size; i++)
    {
        checksum += compressed_data[i];
    }
    printf("Compressed data checksum: 0x%08X\r\n", checksum);
    noprint_output = checksum;
    (void)noprint_output;

#if HOST_TEST
    printf("Writing compressed data to %s\r\n", OUTPUT_FILENAME);
    printf("Diff with src/lzfx_decomp/input.h to verify correctness\r\n");

    FILE *fp = fopen(OUTPUT_FILENAME, "wt");
    if (fp == NULL)
    {
        printf("Error creating compressed_data.h\r\n");
        return 1;
    }
    fprintf(fp, "#include <stdint.h>\n\n");
    fprintf(fp, "uint8_t input_data[] =\n");
    fprintf(fp, "{\n    ");
    for (uint32_t i = 0; i < compressed_size; i++)
    {
        fprintf(fp, "0x%02X,%s", compressed_data[i], (i & 7) == 7 ? "\n    " : " ");
    }
    fprintf(fp, "\n");
    fprintf(fp, "};\n");
    fclose(fp);
#endif

    return benchmark_hash_ret;
}
