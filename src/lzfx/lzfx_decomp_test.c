#include <stdint.h>
#include "def_data.h"

extern void hexstring(uint32_t d);

int32_t lzfx_decompress_tiny(
    const uint8_t *ibuf,
    const uint32_t ilen,
    uint8_t *obuf,
    uint32_t *olen);

uint32_t inf_data_len;
uint8_t inf_data[0x4000];

int32_t lzfx_decompress_test(void)
{
    int32_t ret;
    uint32_t ra, rb;

    inf_data_len = sizeof(inf_data);
    ret = lzfx_decompress_tiny(def_data, DEF_DATA_LEN, inf_data, &inf_data_len);
    if (ret)
    {
        return (ret);
    }
    hexstring(inf_data_len);
    if (inf_data_len != INF_DATA_LEN)
    {
        return (1);
    }
    rb = 0;
    for (ra = 0; ra < inf_data_len; ra++)
    {
        rb += inf_data[ra];
    }
    rb &= 0xFFFF;
    hexstring(rb);
    if (rb != INF_DATA_CHECKSUM)
    {
        return (1);
    }

    return (0);
}

int32_t benchmark_main(void)
{
    hexstring(0x1234);
    if (lzfx_decompress_test())
    {
        hexstring(0x0BAD);
    }
    hexstring(0x1234);

    return (0);
}
