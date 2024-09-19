#include <stdio.h>
#include <string.h>

#include "rsa_test.h"
#include "bn.h"
#include "common.h"

char resultBuffer[8192];

const char public_key[] =
    "b7a6084628c283c8dbc3d5ce296df8e7084372e85ed2e59a602e4a5033c08b44"
    "785da139a50e044a393bfcc9fb3e979f71ec068df6fee305207f2c391fcd7f12"
    "db3ad527073a6a47a64822d22edcb7185660c06dedd2a4ccadb245ec2c16a4aa"
    "3b6a2056c0f1d87ef4c81de16f646805f4fef38e28d70e028dd38640f30a9887";

const char private_key[] =
    "a8821a397610f5270a2f69311fe69e6a2eb4e4ecbe8538d42bc88dbb85a0f8a8"
    "bce0fc9c8e0c8c7726891a6ca2b24e21a9406ecb3885abfcfa52765b7c08ecbc"
    "ebd7e4fbe88cc668f533b62de3a0890843e96f7687bd50d3bd100ac777179b43"
    "1cb5c06be66bb515e89eed7ea9233a26dda1d89127d23ec6fbc306b98b1d8961";

// "ABCDEFHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,." * 2
const char plaintext[] =
    "4142434445464748494a4b4c4d4e4f505152535455565758595a616263646566"
    "6768696a6b6c6d6e6f707172737475767778797a313233343536373839302c2e"
    "4142434445464748494a4b4c4d4e4f505152535455565758595a616263646566"
    "6768696a6b6c6d6e6f707172737475767778797a313233343536373839302c2e";

const char ciphertext[] =
    "5136258167b0c0f43e982a1948006b174f551f1d72b65acfe44dd55b98370587"
    "f87a4fa1cc6b4bf78b009c57d4c94dac2550cdb5421922a296373db63051da27"
    "4aaf595b3b359542ecba549e9ce239811ad250cc098c1518874a2be17ae082d6"
    "912b38e0b9aa4045a8fd6805f0b0c365e6075464cc394b04a8dca0d583f0ca98";

const uint64_t public_exponent = 65537;


int check_encrypt()
{
    return memcmp((char*) ciphertext, (char*) resultBuffer, sizeof(ciphertext));
}

int check_decrypt()
{
    return memcmp((char*) plaintext, (char*) resultBuffer, strlen(plaintext));
}

int benchmark_main(void)
{
    rsa1024_encrypt(public_key, public_exponent, plaintext, resultBuffer);
    rsa1024_decrypt(public_key, private_key, ciphertext, resultBuffer);

    return 0;
}
