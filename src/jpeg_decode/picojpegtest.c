#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "picojpeg.h"
#include "jpeg_input.h"
#include "common.h"

#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

static const char* scan_type_strings[] = {
    "GRAYSCALE",
    "YH1V1",
    "YH2V1",
    "YH1V2",
    "YH2V2"
};

static uint32_t g_FakeFileSize;
static uint32_t g_FakeFileOffset;

uint8_t pjpeg_need_bytes_callback(uint8_t *pBuf, uint8_t buf_size, uint8_t *pBytes_actually_read, void *pCallback_data)
{
    uint32_t to_read;

    to_read = min(g_FakeFileSize - g_FakeFileOffset, buf_size);
    memcpy(pBuf, &test_data[g_FakeFileOffset], to_read);
    *pBytes_actually_read = (uint8_t)(to_read);
    g_FakeFileOffset += to_read;
    return 0;
}

int benchmark_main()
{
    pjpeg_image_info_t image_info;
    int mcu_x = 0;
    int mcu_y = 0;
    uint8_t status;
    uint32_t r_checksum = 0;
    uint32_t g_checksum = 0;
    uint32_t b_checksum = 0;

    g_FakeFileOffset = 0;
    g_FakeFileSize = sizeof(test_data);

    status = pjpeg_decode_init(&image_info, pjpeg_need_bytes_callback, NULL, 0);

    // Print all fields of image_info
    printf("Image width: %d\r\n", image_info.m_width);
    printf("Image height: %d\r\n", image_info.m_height);
    printf("Components per pixel: %d (%s)\r\n", image_info.m_comps, image_info.m_comps == 1 ? "grayscale" : "color");
    printf("MCUs per row: %d\r\n", image_info.m_MCUSPerRow);
    printf("MCUs per column: %d\r\n", image_info.m_MCUSPerCol);
    printf("Scan type: %s\r\n", scan_type_strings[image_info.m_scanType]);
    printf("MCU width: %d\r\n", image_info.m_MCUWidth);
    printf("MCU height: %d\r\n", image_info.m_MCUHeight);

    if (status)
    {
        printf("pjpeg_decode_init() failed with status %u\n", status);
        return status;
    }

#if HOST_TEST
    uint32_t row_pitch = image_info.m_width * image_info.m_comps;
    uint8_t* pImage = (uint8_t *)malloc(row_pitch * image_info.m_height);
    if (!pImage)
    {
        return -1;
    }
#endif

    while (1)
    {
        int y, x;

        status = pjpeg_decode_mcu();

        if (status)
        {
            if (status != PJPG_NO_MORE_BLOCKS)
            {
                printf("pjpeg_decode_mcu() failed with status %u\n", status);
                return status;
            }

            break;
        }

        if (mcu_y >= image_info.m_MCUSPerCol)
        {
            return -1;
        }

#if HOST_TEST
        // Copy MCU's pixel blocks into the destination bitmap.
        uint8_t* pDst_row = pImage + (mcu_y * image_info.m_MCUHeight) * row_pitch + (mcu_x * image_info.m_MCUWidth * image_info.m_comps);
#endif  // HOST_TEST

        for (y = 0; y < image_info.m_MCUHeight; y += 8)
        {
            const int by_limit = min(8, image_info.m_height - (mcu_y * image_info.m_MCUHeight + y));

            for (x = 0; x < image_info.m_MCUWidth; x += 8)
            {
#if HOST_TEST
                uint8_t *pDst_block = pDst_row + x * image_info.m_comps;
#endif  // HOST_TEST

                // Compute source byte offset of the block in the decoder's MCU buffer.
                uint32_t src_ofs = (x * 8U) + (y * 16U);
                const uint8_t *pSrcR = image_info.m_pMCUBufR + src_ofs;
                const uint8_t *pSrcG = image_info.m_pMCUBufG + src_ofs;
                const uint8_t *pSrcB = image_info.m_pMCUBufB + src_ofs;

                const int bx_limit = min(8, image_info.m_width - (mcu_x * image_info.m_MCUWidth + x));

                int bx, by;
                for (by = 0; by < by_limit; by++)
                {
#if HOST_TEST
                    uint8_t *pDst = pDst_block;
#endif  // HOST_TEST

                    for (bx = 0; bx < bx_limit; bx++)
                    {
#if HOST_TEST
                        pDst[0] = *pSrcR;
                        pDst[1] = *pSrcG;
                        pDst[2] = *pSrcB;
                        pDst += 3;
#endif  // HOST_TEST
                        r_checksum += *pSrcR++;
                        g_checksum += *pSrcG++;
                        b_checksum += *pSrcB++;
                    }

                    pSrcR += (8 - bx_limit);
                    pSrcG += (8 - bx_limit);
                    pSrcB += (8 - bx_limit);

#if HOST_TEST
                    pDst_block += row_pitch;
#endif  // HOST_TEST
                }
            }
#if HOST_TEST
            pDst_row += (row_pitch * 8);
#endif  // HOST_TEST
        }

        mcu_x++;
        if (mcu_x == image_info.m_MCUSPerRow)
        {
            mcu_x = 0;
            mcu_y++;
        }
    }

    printf("R checksum: %u\r\n", r_checksum);
    printf("G checksum: %u\r\n", g_checksum);
    printf("B checksum: %u\r\n", b_checksum);
    (void)r_checksum;
    (void)g_checksum;
    (void)b_checksum;

#if HOST_TEST
    FILE *fpout = fopen("out.raw", "wb");
    if (fpout)
    {
        fwrite(pImage, 1, image_info.m_width * image_info.m_height * image_info.m_comps, fpout);
        fclose(fpout);
    }
    printf("Image written to out.raw\r\n");
#endif  // HOST_TEST

    return 0;
}
