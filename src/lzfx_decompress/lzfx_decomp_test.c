#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "lzfx.h"
#include "input.h"

#define OUTPUT_FILENAME "lzfx_decompressed_data.txt"
#define DECOMP_DATA_LEN 0x2000
uint8_t decompressed_data[DECOMP_DATA_LEN];

int benchmark_main(void)
{
    uint32_t compressed_size = sizeof(input_data);
    uint32_t decompressed_size = DECOMP_DATA_LEN;

    printf("Decompressing %u bytes of data...\r\n", compressed_size);
    int32_t ret = lzfx_decompress_tiny(input_data, compressed_size, decompressed_data, &decompressed_size);

    if (ret < 0)
    {
        printf("Compression failed with error code %d\r\n", ret);
        return 1;
    }

    printf("Decompressed size: %u bytes\r\n", decompressed_size);

    // compute checksum on decompressed data
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < compressed_size; i++)
    {
        checksum += decompressed_data[i];
    }
    printf("Decompressed data checksum: 0x%08X\r\n", checksum);

#if HOST_TEST
    printf("Writing decompressed data to %s\r\n", OUTPUT_FILENAME);
    printf("Diff with src/lzfx_compress/dickens.txt to verify correctness\r\n");

    FILE *fp = fopen(OUTPUT_FILENAME, "wt");
    fwrite(decompressed_data, 1, decompressed_size, fp);
    fclose(fp);
#endif  // HOST_TEST

    return 0;
}
