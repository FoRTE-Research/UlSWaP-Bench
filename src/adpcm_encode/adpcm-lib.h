////////////////////////////////////////////////////////////////////////////
//                           **** ADPCM-XQ ****                           //
//                  Xtreme Quality ADPCM Encoder/Decoder                  //
//                    Copyright (c) 2024 David Bryant.                    //
//                          All Rights Reserved.                          //
//      Distributed under the BSD Software License (see license.txt)      //
////////////////////////////////////////////////////////////////////////////

#ifndef ADPCMLIB_H_
#define ADPCMLIB_H_

#define NOISE_SHAPING_OFF       0   // flat noise (no shaping)
#define NOISE_SHAPING_STATIC    1   // first-order highpass shaping
#define NOISE_SHAPING_DYNAMIC   2   // dynamically tilted noise based on signal

#define LOOKAHEAD_DEPTH         0x0ff   // depth of search
#define LOOKAHEAD_EXHAUSTIVE    0x800   // full breadth of search

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef unsigned __int64 uint64_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int8 uint8_t;
typedef __int64 int64_t;
typedef __int32 int32_t;
typedef __int16 int16_t;
typedef __int8  int8_t;
#else
#include <stdint.h>
#endif

#ifdef __cplusplus 
extern "C" {
#endif


void *adpcm_create_context (int num_channels, int lookahead, int noise_shaping);
void adpcm_set_shaping_weight (void *p, double shaping_weight);
int adpcm_encode_block (void *p, uint8_t *outbuf, size_t *outbufsize, const int16_t *inbuf, int inbufcount);
int adpcm_decode_block_ex (int16_t *outbuf, const uint8_t *inbuf, size_t inbufsize, int channels, int bps);
int adpcm_decode_block (int16_t *outbuf, const uint8_t *inbuf, size_t inbufsize, int channels);

#ifdef __cplusplus 
}
#endif


#endif /* ADPCMLIB_H_ */
