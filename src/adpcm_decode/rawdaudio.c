/* testd - Test adpcm decoder */

#include "adpcm.h"
#include <stdio.h>
#include "common.h"
#include "input.h"

#define NSAMPLES 1000
#define NINC (NSAMPLES / 2)

int16_t sbuf[NSAMPLES];

int32_t benchmark_main()
{
    struct adpcm_state state = {};
    int32_t n = 0;
    uint8_t *currentN = test_data;
    uint32_t maxN = sizeof(test_data);

    printf("Initial valprev=%d, index=%d\r\n", state.valprev, state.index);
    printf("Decoding %d bytes\r\n", maxN);

    while (1)
    {
        uint32_t bytesIntoRead = currentN - test_data;
        uint32_t testN = bytesIntoRead + NINC;
        n = (testN <= maxN) ? NINC : maxN - bytesIntoRead;

        if (n == 0)
        {
            break;
        }

        adpcm_decoder(currentN, sbuf, n * 2, &state);
        currentN = test_data + bytesIntoRead + n;
    }

    printf("Final valprev=%d, index=%d\r\n", state.valprev, state.index);
    return 0;
}
