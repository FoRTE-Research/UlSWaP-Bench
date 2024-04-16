#include <stdio.h>
#include <stdint.h>
#include "picojpeg.h"
#include "g_jpegdata.h"

void hexstring(uint32_t d);

uint32_t doff;

uint8_t pjpeg_need_bytes_callback(uint8_t *pBuf, uint8_t buf_size, uint8_t *pBytes_actually_read, void *pCallback_data)
{
    uint32_t ra;
    //   uint n;
    // pCallback_data;

    //   n = min(g_nInFileSize - g_nInFileOfs, buf_size);
    // if (n && (fread(pBuf, 1, n, g_pInFile) != n))
    // return PJPG_STREAM_READ_ERROR;
    //*pBytes_actually_read = (uint8_t)(n);
    // g_nInFileOfs += n;

    // memcpy(pBuf,&jpegdata[doff],buf_size);
    for (ra = 0; ra < buf_size; ra++)
        pBuf[ra] = jpegdata[doff + ra];
    doff += buf_size;
    *pBytes_actually_read = buf_size;
    return 0;
}

int32_t main(void)
{
    uint8_t status;
    pjpeg_image_info_t pInfo;
    uint32_t sum;
    uint32_t rb;

    doff = 0;

    status = pjpeg_decode_init(&pInfo, pjpeg_need_bytes_callback, (void *)0, 0);

    if (status)
    {
        return (1);
    }
    sum = 0;
    while (status == 0)
    {
        status = pjpeg_decode_mcu();
        // printf("decode %u\n",ra);
        if (status == 0)
        {
            switch (pInfo.m_scanType)
            {
            case PJPG_GRAYSCALE:
                break;
            case PJPG_YH1V1:
            {
                for (rb = 0; rb < 64; rb++)
                {
                    sum += pInfo.m_pMCUBufB[rb];
                    sum += pInfo.m_pMCUBufG[rb];
                    sum += pInfo.m_pMCUBufR[rb];
                }
                //// PJPG_H1V1: Each MCU contains is decoded to a single block of 8x8 RGB pixels.
                ////   uint8_t *m_pMCUBufR;
                // for(rb=0;rb<64;rb++) sum+=pInfo.m_pMCUBufR[rb];
                // fwrite(pInfo.m_pMCUBufR,1,64,fpout);
                // fwrite(pInfo.m_pMCUBufR,1,64,fp);
                ////uint8_t *m_pMCUBufG;
                // for(rb=0;rb<64;rb++) sum+=pInfo.m_pMCUBufG[rb];
                // fwrite(pInfo.m_pMCUBufG,1,64,fpout);
                // fwrite(pInfo.m_pMCUBufG,1,64,fp);
                ////uint8_t *m_pMCUBufB;
                // for(rb=0;rb<64;rb++) sum+=pInfo.m_pMCUBufB[rb];
                // fwrite(pInfo.m_pMCUBufB,1,64,fpout);
                // fwrite(pInfo.m_pMCUBufB,1,64,fp);
                break;
            }
            case PJPG_YH2V1:
                break;
            case PJPG_YH1V2:
                break;
            case PJPG_YH2V2:
                break;
            }
        }
    }
    if (status != PJPG_NO_MORE_BLOCKS)
    {
        return (1);
    }
    hexstring(sum);
    return (0);
}
