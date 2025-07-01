////////////////////////////////////////////////////////////////////////////
//                           **** ADPCM-XQ ****                           //
//                  Xtreme Quality ADPCM Encoder/Decoder                  //
//                    Copyright (c) 2024 David Bryant.                    //
//                          All Rights Reserved.                          //
//      Distributed under the BSD Software License (see license.txt)      //
////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "common.h"
#include "adpcm-lib.h"
#include "input.h"

// This runtime macro is not strictly needed because the code is endian-safe,
// but including it improves performance on little-endian systems because we
// can avoid a couple loops through the audio.
#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x0100)

#define ADPCM_FLAG_NOISE_SHAPING 0x1
#define ADPCM_FLAG_RAW_OUTPUT 0x2
#define ADPCM_FLAG_MEASURE_NOISE 0x4

#if HOST_TEST
#define OUTPUT_FILENAME "output.wav"
static FILE *g_pOutfile;
#endif  // HOST_TEST

static uint32_t adpcm_converter();
static int32_t flags = ADPCM_FLAG_NOISE_SHAPING;
static int32_t lookahead = 3;
static int32_t blocksize_pow2 = 0;
static double static_shaping_weight = 0.0;

benchmark_hash_t benchmark_main(void)
{
    benchmark_hash_t benchmark_hash_ret = 0;

    flags &= ~ADPCM_FLAG_NOISE_SHAPING;
    static_shaping_weight = 0.0;
    adpcm_converter();

    return benchmark_hash_ret;
}

typedef struct
{
    char ckID[4];
    uint32_t ckSize;
    char formType[4];
} RiffChunkHeader;

typedef struct
{
    char ckID[4];
    uint32_t ckSize;
} ChunkHeader;

#define ChunkHeaderFormat "4L"

typedef struct
{
    uint16_t FormatTag, NumChannels;
    uint32_t SampleRate, BytesPerSecond;
    uint16_t BlockAlign, BitsPerSample;
    uint16_t cbSize;
    union
    {
        uint16_t ValidBitsPerSample;
        uint16_t SamplesPerBlock;
        uint16_t Reserved;
    } Samples;
    int32_t ChannelMask;
    uint16_t SubFormat;
    char GUID[14];
} WaveHeader;

#define WaveHeaderFormat "SSLLSSSSLS"

typedef struct
{
    char ckID[4];
    uint32_t ckSize;
    uint32_t TotalSamples;
} FactHeader;

#define FactHeaderFormat "4LL"

#define WAVE_FORMAT_PCM 0x1
#define WAVE_FORMAT_IMA_ADPCM 0x11
#define WAVE_FORMAT_EXTENSIBLE 0xfffe

static uint32_t write_adpcm_wav_header(int32_t num_channels, uint32_t num_samples, uint32_t sample_rate, int32_t samples_per_block, uint32_t* checksum);
static uint32_t adpcm_encode_data(uint8_t *input_file_buf, int32_t num_channels, uint32_t num_samples, int32_t samples_per_block, int32_t sample_rate, uint32_t* checksum);
static void little_endian_to_native(void *data, char *format);
static void native_to_little_endian(void *data, char *format);

static uint32_t adpcm_converter()
{
    volatile uint32_t noprint_output;
    uint32_t checksum = 0;
    uint32_t bytes_written = 0;
    int32_t format = 0;
    int32_t bits_per_sample = 0;
    int32_t num_channels;
    uint32_t fact_samples = 0;
    uint32_t num_samples = 0;
    uint32_t sample_rate;
    RiffChunkHeader *riff_chunk_header;
    ChunkHeader *chunk_header;
    WaveHeader *pWaveHeader;

    uint8_t* input_file_buf = (uint8_t*)test_data;
    printf("Input file size = %zu bytes\r\n", sizeof(test_data));

    // Read initial RIFF form header
    riff_chunk_header = (RiffChunkHeader *)test_data;
    input_file_buf += sizeof(RiffChunkHeader);
    if (strncmp(riff_chunk_header->ckID, "RIFF", 4) ||
        strncmp(riff_chunk_header->formType, "WAVE", 4))
    {
        printf("Input is not a valid .WAV file!\r\n");
        return -1;
    }

    // Loop through all elements of the RIFF wav header (until the data chuck)
    while (1)
    {
        chunk_header = (ChunkHeader *)input_file_buf;
        input_file_buf += sizeof(ChunkHeader);

        little_endian_to_native(&chunk_header, ChunkHeaderFormat);

        // If it's the format chunk, we want to get some info out of there and make sure it's a .wav file we can handle
        if (!strncmp(chunk_header->ckID, "fmt ", 4))
        {
            int32_t supported = 1;

            if (chunk_header->ckSize < 16 || chunk_header->ckSize > sizeof(*pWaveHeader))
            {
                printf("Input is not a valid .WAV file!\r\n");
                return -1;
            }

            pWaveHeader = (WaveHeader *)input_file_buf;
            input_file_buf += chunk_header->ckSize;

            little_endian_to_native(&pWaveHeader, WaveHeaderFormat);

            format = (pWaveHeader->FormatTag == WAVE_FORMAT_EXTENSIBLE && chunk_header->ckSize == 40) ? pWaveHeader->SubFormat : pWaveHeader->FormatTag;

            bits_per_sample = (chunk_header->ckSize == 40 && pWaveHeader->Samples.ValidBitsPerSample) ? pWaveHeader->Samples.ValidBitsPerSample : pWaveHeader->BitsPerSample;

            if (pWaveHeader->NumChannels < 1 || pWaveHeader->NumChannels > 2)
            {
                supported = 0;
            }
            else if (format == WAVE_FORMAT_PCM)
            {

                if (bits_per_sample < 9 || bits_per_sample > 16)
                {
                    supported = 0;
                }

                if (pWaveHeader->BlockAlign != pWaveHeader->NumChannels * 2)
                {
                    supported = 0;
                }
            }
            else
                supported = 0;

            if (!supported)
            {
                printf("Input is an unsupported .WAV format!\r\n");
                return -1;
            }

            printf("Input channels = %u\r\n", pWaveHeader->NumChannels);
            printf("Input bits per sample = %u\r\n", pWaveHeader->BitsPerSample);
            printf("Input sample rate = %u\r\n", pWaveHeader->SampleRate);
        }
        else if (!strncmp(chunk_header->ckID, "fact", 4))
        {
            memcpy(&fact_samples, input_file_buf, sizeof(fact_samples));
            input_file_buf += sizeof(fact_samples);

            little_endian_to_native(&fact_samples, "L");

            if (chunk_header->ckSize > 4)
            {
                int32_t bytes_to_skip = chunk_header->ckSize - 4;

                while (bytes_to_skip--)
                {
                    input_file_buf++;
                }
            }
        }
        else if (!strncmp(chunk_header->ckID, "data", 4))
        {
            // On the data chunk, get size and exit parsing loop
            if (!pWaveHeader->NumChannels)
            {
                printf("Input is not a valid .WAV file!\r\n");
                return -1;
            }

            if (!chunk_header->ckSize)
            {
                printf("This .WAV file has no audio samples, probably is corrupt!\r\n");
                return -1;
            }

            if (format == WAVE_FORMAT_PCM)
            {
                if (chunk_header->ckSize % pWaveHeader->BlockAlign)
                {
                    printf("Input is not a valid .WAV file!\r\n");
                    return -1;
                }

                num_samples = chunk_header->ckSize / pWaveHeader->BlockAlign;
            }
            else
            {
                uint32_t complete_blocks = chunk_header->ckSize / pWaveHeader->BlockAlign;
                int32_t leftover_bytes = chunk_header->ckSize % pWaveHeader->BlockAlign;
                int32_t samples_last_block;

                num_samples = complete_blocks * pWaveHeader->Samples.SamplesPerBlock;

                if (leftover_bytes)
                {
                    if (leftover_bytes % (pWaveHeader->NumChannels * 4))
                    {
                        printf("Input is not a valid .WAV file!\r\n");
                        return -1;
                    }

                    printf("Data chunk has %d bytes left over for final ADPCM block\r\n", leftover_bytes);
                    samples_last_block = ((leftover_bytes - (pWaveHeader->NumChannels * 4)) * 8) / (bits_per_sample * pWaveHeader->NumChannels) + 1;
                    num_samples += samples_last_block;
                }
                else
                {
                    samples_last_block = pWaveHeader->Samples.SamplesPerBlock;
                }

                if (fact_samples)
                {
                    if (fact_samples < num_samples && fact_samples > num_samples - samples_last_block)
                    {
                        printf("total samples reduced %u by FACT chunk\r\n", (num_samples - fact_samples));
                        num_samples = fact_samples;
                    }
                    else if (pWaveHeader->NumChannels == 2 && (fact_samples >>= 1) < num_samples && fact_samples > num_samples - samples_last_block)
                    {
                        printf("num samples reduced %u by [incorrect] FACT chunk\r\n", (num_samples - fact_samples));
                        num_samples = fact_samples;
                    }
                }
            }

            if (!num_samples)
            {
                printf("This .WAV file has no audio samples, probably is corrupt!\r\n");
                return -1;
            }

            printf("Total input samples = %u\r\n", num_samples);

            num_channels = pWaveHeader->NumChannels;
            sample_rate = pWaveHeader->SampleRate;
            break;
        }
        else
        {
            // just ignore unknown chunks
            int32_t bytes_to_eat = (chunk_header->ckSize + 1) & ~1L;

            printf("extra unknown chunk \"%c%c%c%c\" of %d bytes\r\n",
                   chunk_header->ckID[0], chunk_header->ckID[1], chunk_header->ckID[2],
                   chunk_header->ckID[3], chunk_header->ckSize);

            while (bytes_to_eat--)
            {
                input_file_buf++;
            }
        }
    }

#if HOST_TEST
    g_pOutfile = fopen(OUTPUT_FILENAME, "wb");
    if (!g_pOutfile)
    {
        printf("Can't open file %s for writing!\r\n", OUTPUT_FILENAME);
        return -1;
    }
#endif // HOST_TEST

    if (format == WAVE_FORMAT_PCM)
    {
        int32_t block_size, samples_per_block;

        if (blocksize_pow2)
            block_size = 1 << blocksize_pow2;
        else
            block_size = 256 * num_channels * (sample_rate < 11000 ? 1 : sample_rate / 11000);

        samples_per_block = (block_size - num_channels * 4) * (num_channels ^ 3) + 1;

        printf("Each %d byte ADPCM block will contain %d samples\r\n",
               block_size, samples_per_block);

        bytes_written += write_adpcm_wav_header(num_channels, num_samples, sample_rate, samples_per_block, &checksum);
        bytes_written += adpcm_encode_data(input_file_buf, num_channels, num_samples, samples_per_block, sample_rate, &checksum);
    }

    printf("Output file size = %u bytes\r\n", bytes_written);
    printf("Checksum = %u\r\n", checksum);
    noprint_output = bytes_written;
    noprint_output = checksum;
    (void)noprint_output;

#if HOST_TEST
    printf("Output written to %s\r\n", OUTPUT_FILENAME);
    fclose(g_pOutfile);
#endif // HOST_TEST

    return checksum;
}

static uint32_t add_or_write_buffer(void *buffer, uint32_t size)
{
#if HOST_TEST
    fwrite(buffer, size, 1, g_pOutfile);
#endif // HOST_TEST

    uint32_t checksum = 0;

    for (uint32_t i = 0; i < size; i++)
    {
        checksum += ((uint8_t *)buffer)[i];
    }

    return checksum;
}

static uint32_t write_adpcm_wav_header(int32_t num_channels, uint32_t num_samples, uint32_t sample_rate, int32_t samples_per_block, uint32_t* checksum)
{
    RiffChunkHeader riffhdr;
    ChunkHeader datahdr, fmthdr;
    WaveHeader wavhdr;
    FactHeader facthdr;

    uint32_t bytes_written = 0;
    int32_t wavhdrsize = 20;
    int32_t block_size = (samples_per_block - 1) / (num_channels ^ 3) + (num_channels * 4);
    uint32_t num_blocks = num_samples / samples_per_block;
    int32_t leftover_samples = num_samples % samples_per_block;
    uint32_t total_data_bytes = num_blocks * block_size;

    if (leftover_samples)
    {
        int32_t last_block_samples = ((leftover_samples + 6) & ~7) + 1;
        int32_t last_block_size = (last_block_samples - 1) / (num_channels ^ 3) + (num_channels * 4);
        total_data_bytes += last_block_size;
    }

    memset(&wavhdr, 0, sizeof(wavhdr));

    wavhdr.FormatTag = WAVE_FORMAT_IMA_ADPCM;
    wavhdr.NumChannels = num_channels;
    wavhdr.SampleRate = sample_rate;
    wavhdr.BytesPerSecond = sample_rate * block_size / samples_per_block;
    wavhdr.BlockAlign = block_size;
    wavhdr.BitsPerSample = 4;
    wavhdr.cbSize = 2;
    wavhdr.Samples.SamplesPerBlock = samples_per_block;

    strncpy(riffhdr.ckID, "RIFF", sizeof(riffhdr.ckID));
    strncpy(riffhdr.formType, "WAVE", sizeof(riffhdr.formType));
    riffhdr.ckSize = sizeof(riffhdr) + wavhdrsize + sizeof(facthdr) + sizeof(datahdr) + total_data_bytes;
    strncpy(fmthdr.ckID, "fmt ", sizeof(fmthdr.ckID));
    fmthdr.ckSize = wavhdrsize;
    strncpy(facthdr.ckID, "fact", sizeof(facthdr.ckID));
    facthdr.TotalSamples = num_samples;
    facthdr.ckSize = 4;

    strncpy(datahdr.ckID, "data", sizeof(datahdr.ckID));
    datahdr.ckSize = total_data_bytes;

    // write the RIFF chunks up to just before the data starts

    native_to_little_endian(&riffhdr, ChunkHeaderFormat);
    native_to_little_endian(&fmthdr, ChunkHeaderFormat);
    native_to_little_endian(&wavhdr, WaveHeaderFormat);
    native_to_little_endian(&facthdr, FactHeaderFormat);
    native_to_little_endian(&datahdr, ChunkHeaderFormat);

    *checksum += add_or_write_buffer(&riffhdr, sizeof(riffhdr));
    *checksum += add_or_write_buffer(&fmthdr, sizeof(fmthdr));
    *checksum += add_or_write_buffer(&wavhdr, wavhdrsize);
    *checksum += add_or_write_buffer(&facthdr, sizeof(facthdr));
    *checksum += add_or_write_buffer(&datahdr, sizeof(datahdr));

    bytes_written += sizeof(riffhdr) + sizeof(fmthdr) + wavhdrsize + sizeof(facthdr) + sizeof(datahdr);

    return bytes_written;
}

static uint32_t adpcm_encode_data(uint8_t *input_file_buf, int32_t num_channels, uint32_t num_samples, int32_t samples_per_block, int32_t sample_rate, uint32_t* checksum)
{
    int32_t block_size = (samples_per_block - 1) / (num_channels ^ 3) + (num_channels * 4);
    int16_t *pcm_block = alloca(samples_per_block * num_channels * 2);
    void *adpcm_block = alloca(block_size);
    void *adpcm_cnxt = NULL;
    uint32_t bytes_written = 0;

    if (!pcm_block || !adpcm_block)
    {
        printf("could not allocate memory for buffers!\r\n");
        return -1;
    }

    while (num_samples)
    {
        int32_t this_block_adpcm_samples = samples_per_block;
        int32_t this_block_pcm_samples = samples_per_block;
        size_t num_bytes;

        if (this_block_pcm_samples > num_samples)
        {
            this_block_adpcm_samples = ((num_samples + 6) & ~7) + 1;
            block_size = (this_block_adpcm_samples - 1) / (num_channels ^ 3) + (num_channels * 4);
            this_block_pcm_samples = num_samples;
        }

        memcpy(pcm_block, input_file_buf, this_block_pcm_samples * num_channels * 2);
        input_file_buf += this_block_pcm_samples * num_channels * 2;

        if (IS_BIG_ENDIAN)
        {
            int32_t scount = this_block_pcm_samples * num_channels;
            uint8_t *cp = (uint8_t *)pcm_block;

            while (scount--)
            {
                int16_t temp = cp[0] + (cp[1] << 8);
                *(int16_t *)cp = temp;
                cp += 2;
            }
        }

        // if this is the last block and it's not full, duplicate the last sample(s) so we don't
        // create problems for the lookahead

        if (this_block_adpcm_samples > this_block_pcm_samples)
        {
            int16_t *dst = pcm_block + this_block_pcm_samples * num_channels;
            int16_t *src = dst - num_channels;
            int32_t dups = (this_block_adpcm_samples - this_block_pcm_samples) * num_channels;

            while (dups--)
                *dst++ = *src++;
        }

        if (!adpcm_cnxt)
        {
            int32_t noise_shaping;

            if (flags & ADPCM_FLAG_NOISE_SHAPING)
            {
                if (static_shaping_weight != 0.0)
                    noise_shaping = NOISE_SHAPING_STATIC;
                else if (sample_rate > 64000)
                {
                    noise_shaping = NOISE_SHAPING_STATIC;
                    static_shaping_weight = 1.0;
                }
                else
                    noise_shaping = NOISE_SHAPING_DYNAMIC;
            }
            else
                noise_shaping = NOISE_SHAPING_OFF;

            adpcm_cnxt = adpcm_create_context(num_channels, lookahead, noise_shaping);

            if (noise_shaping == NOISE_SHAPING_STATIC)
                adpcm_set_shaping_weight(adpcm_cnxt, static_shaping_weight);
        }

        adpcm_encode_block(adpcm_cnxt, adpcm_block, &num_bytes, pcm_block, this_block_adpcm_samples);

        if (num_bytes != block_size)
        {
            printf("\radpcm_encode_block() did not return expected value (expected %d, got %d)!\r\n", block_size, (int32_t)num_bytes);
            return -1;
        }

        *checksum += add_or_write_buffer(adpcm_block, block_size);
        bytes_written += block_size;

        num_samples -= this_block_pcm_samples;
    }

    return bytes_written;
}

static void little_endian_to_native(void *data, char *format)
{
    uint8_t *cp = (uint8_t *)data;
    int32_t temp;

    while (*format)
    {
        switch (*format)
        {
        case 'L':
            temp = cp[0] + ((int32_t)cp[1] << 8) + ((int32_t)cp[2] << 16) + ((int32_t)cp[3] << 24);
            *(int32_t *)cp = temp;
            cp += 4;
            break;

        case 'S':
            temp = cp[0] + (cp[1] << 8);
            *(int16_t *)cp = (int16_t)temp;
            cp += 2;
            break;

        default:
            if (isdigit((uint8_t)*format))
                cp += *format - '0';

            break;
        }

        format++;
    }
}

static void native_to_little_endian(void *data, char *format)
{
    uint8_t *cp = (uint8_t *)data;
    int32_t temp;

    while (*format)
    {
        switch (*format)
        {
        case 'L':
            temp = *(int32_t *)cp;
            *cp++ = (uint8_t)temp;
            *cp++ = (uint8_t)(temp >> 8);
            *cp++ = (uint8_t)(temp >> 16);
            *cp++ = (uint8_t)(temp >> 24);
            break;

        case 'S':
            temp = *(int16_t *)cp;
            *cp++ = (uint8_t)temp;
            *cp++ = (uint8_t)(temp >> 8);
            break;

        default:
            if (isdigit((uint8_t)*format))
                cp += *format - '0';

            break;
        }

        format++;
    }
}
