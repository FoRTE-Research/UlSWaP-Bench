/*
** adpcm.h - include file for adpcm coder.
**
** Version 1.0, 7-Jul-92.
*/

#include <stdint.h>

struct adpcm_state
{
    int16_t valprev; /* Previous output value */
    int8_t  index;    /* Index into stepsize table */
};


void adpcm_coder (uint8_t[], int8_t[], int32_t, struct adpcm_state *);
void adpcm_decoder (uint8_t[], int16_t[], int32_t, struct adpcm_state *);
