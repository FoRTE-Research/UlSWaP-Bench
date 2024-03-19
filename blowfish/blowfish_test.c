/*
blowfish_test.c:  Test file for blowfish.c

Copyright (C) 1997 by Paul Kocher

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include "blowfish.h"
#include "../bareBench.h"
#include "input.h"

unsigned char KEY[] = "1234567890abcdeffedcba0987654321";

int main(void)
{
    uint32_t L = 1, R = 2;
    printf("size = %zu\r\n", sizeof(L));
    BLOWFISH_CTX ctx;

    Blowfish_Init(&ctx, (unsigned char *)"TESTKEY", 7);

    Blowfish_Encrypt(&ctx, &L, &R);
    printf("%08X %08X\r\n", L, R);
    if (L == 0xDF333FD2L && R == 0x30A71BB4L)
        printf("Test encryption OK.\r\n");
    else
        printf("Test encryption failed.\r\n");

    Blowfish_Decrypt(&ctx, &L, &R);
    if (L == 1 && R == 2)
        printf("Test decryption OK.\r\n");
    else
        printf("Test decryption failed.\r\n");

    printf("Encrypt message\r\n");
    Blowfish_Init(&ctx, KEY, sizeof(KEY));

    uint32_t *plaintextPtr = (uint32_t *)test_data;
    while (plaintextPtr < (uint32_t *)(test_data + sizeof(test_data)))
    {
        Blowfish_Encrypt(&ctx, &plaintextPtr[0], &plaintextPtr[1]);
        printf("%08X %08X\r\n", plaintextPtr[0], plaintextPtr[1]);
        plaintextPtr += 2;
    }

    printf("Decrypt message\r\n");
    Blowfish_Init(&ctx, KEY, sizeof(KEY));

    plaintextPtr = (uint32_t *)test_data;
    while (plaintextPtr < (uint32_t *)(test_data + sizeof(test_data)))
    {
        Blowfish_Decrypt(&ctx, &plaintextPtr[0], &plaintextPtr[1]);
        printf("%08X %08X\r\n", plaintextPtr[0], plaintextPtr[1]);
        plaintextPtr += 2;
    }

    return 0;
}
