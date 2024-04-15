#include "../bareBench.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

FILE *fpout;

//------------------------------------------------------------------------
uint32_t prand32(uint32_t x)
{
    if (x & 1)
    {
        x = x >> 1;
        x = x ^ 0xBF9EC099;
    }
    else
    {
        x = x >> 1;
    }
    return (x);
}

int main(void)
{
    uint32_t prand;
    uint32_t ra;
    uint32_t rb;
    uint32_t rc;
    uint32_t rd;

    prand = 0x1234;

    fpout = fopen("g_abcmath.c", "wt");
    if (fpout == NULL)
    {
        return (1);
    }

    fprintf(fpout, "#include <stdint.h>\n\n");
    fprintf(fpout, "uint32_t prand32 ( uint32_t x )\n");
    fprintf(fpout, "{\n");
    fprintf(fpout, "    if(x&1)\n");
    fprintf(fpout, "    {\n");
    fprintf(fpout, "        x=x>>1;\n");
    fprintf(fpout, "        x=x^0xBF9EC099;\n");
    fprintf(fpout, "    }\n");
    fprintf(fpout, "    else\n");
    fprintf(fpout, "    {\n");
    fprintf(fpout, "        x=x>>1;\n");
    fprintf(fpout, "    }\n");
    fprintf(fpout, "    return(x);\n");
    fprintf(fpout, "}\n");
    fprintf(fpout, "\n");
    fprintf(fpout, "uint32_t prand;\n");
    for (ra = 0; ra < 8; ra++)
    {
        fprintf(fpout, "uint32_t %c;\n", 'a' + ra);
    }
    fprintf(fpout, "\n");
    fprintf(fpout, "uint32_t abcmath ( void )\n");
    fprintf(fpout, "{\n");
    for (; ra < 16; ra++)
    {
        fprintf(fpout, "uint32_t %c;\n", 'a' + ra);
    }
    fprintf(fpout, "\n");
    fprintf(fpout, "prand=0xABC;\n");
    for (ra = 0; ra < 16; ra++)
    {
        fprintf(fpout, "prand=prand32(prand); %c=prand;\n", 'a' + ra);
    }
    for (ra = 0; ra < 2000; ra++)
    {
        prand = prand32(prand);
        prand = prand32(prand);
        prand = prand32(prand);
        rb = (prand >> 0) & 15;
        while (1)
        {
            prand = prand32(prand);
            prand = prand32(prand);
            prand = prand32(prand);
            rc = (prand >> 8) & 15;
            if (rb == rc)
            {
                continue;
            }
            break;
        }
        while (1)
        {
            prand = prand32(prand);
            prand = prand32(prand);
            prand = prand32(prand);
            prand = prand32(prand);
            rd = (prand >> 16) & 15;
            if (rb == rc)
            {
                continue;
            }
            if (rb == rd)
            {
                continue;
            }
            break;
        }
        prand = prand32(prand);
        prand = prand32(prand);
        prand = prand32(prand);
        switch ((prand >> 24) & 15)
        {
            case  0: fprintf(fpout,"%c = %c + %c;\n",rb+'a',rc+'a',rd+'a'); break;
            case  1: fprintf(fpout,"%c = %c - %c;\n",rb+'a',rc+'a',rd+'a'); break;
            case  2: fprintf(fpout,"%c = %c * %c;\n",rb+'a',rc+'a',rd+'a'); break;
            case  3: fprintf(fpout,"%c = %c ^ %c;\n",rb+'a',rc+'a',rd+'a'); break;
            case  4: fprintf(fpout,"%c = %c & %c;\n",rb+'a',rc+'a',rd+'a'); break;
            case  5: fprintf(fpout,"%c = %c | %c;\n",rb+'a',rc+'a',rd+'a'); break;
            case  6: fprintf(fpout,"%c = %c + %c;\n",rb+'a',rc+'a',rd+'a'); break;
            case  7: fprintf(fpout,"prand=prand32(prand); %c = prand;\n",rb+'a'); break;
            case  8: fprintf(fpout,"if(%c>%c) { prand=prand32(prand); %c = prand; }\n",rb+'a',rc+'a',rd+'a'); break;
            case  9: fprintf(fpout,"if(%c<%c) { prand=prand32(prand); %c = prand; }\n",rb+'a',rc+'a',rd+'a'); break;
            case 10: fprintf(fpout,"if(%c==%c) { prand=prand32(prand); %c = prand; }\n",rb+'a',rc+'a',rd+'a'); break;
            case 11: fprintf(fpout,"if(%c>=%c) { prand=prand32(prand); %c = prand; }\n",rb+'a',rc+'a',rd+'a'); break;
            case 12: fprintf(fpout,"if(%c<=%c) { prand=prand32(prand); %c = prand; }\n",rb+'a',rc+'a',rd+'a'); break;
        default:
                fprintf(fpout,"%c = %c | %c;\n",rb+'a',rc+'a',rd+'a'); break;
            break;
        }
    }

    for (ra = 1; ra < 16; ra++)
    {
        fprintf(fpout, "a += %c;\n", 'a' + ra);
    }
    fprintf(fpout, "return(a);\n");

    fprintf(fpout, "}\n");
    fprintf(fpout, "\n");
    return (0);
}
