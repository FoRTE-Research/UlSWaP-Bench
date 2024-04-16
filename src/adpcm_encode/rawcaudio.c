/* testc - Test adpcm coder */

#include "adpcm.h"
#include <stdio.h>
#include "../bareBench.h"
#include "input.h"

#define NSAMPLES 1000
#define NINC (NSAMPLES * 2)

int8_t abuf[NSAMPLES / 2];

int main()
{
    struct adpcm_state state = {};
    uint32_t n = 0;
    uint8_t *currentN = test_data;
    uint32_t maxN = sizeof(test_data);

    printf("Initial valprev=%d, index=%d\r\n", state.valprev, state.index);
    printf("Decoding %u bytes\r\n", maxN);
    while (1)
    {
        uint32_t bytesIntoRead = currentN - test_data;
        uint32_t testN = bytesIntoRead + NINC;
        n = (testN <= maxN) ? NINC : maxN - bytesIntoRead;

        if (n == 0)
            break;

        adpcm_coder(currentN, abuf, n / 2, &state);
        currentN = test_data + bytesIntoRead + n;
    }

    printf("Final valprev=%d, index=%d\r\n", state.valprev, state.index);
    return 0;
}
