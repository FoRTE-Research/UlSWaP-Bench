/***************
 STILL IN PRIGRESS
 Comments are left in places where it needs work one.
 Those comments will be removed once relevant information is filled.
 ***************/

/** need to choose which RSA implementation to run **/
#define tiny_rsa
//#define codebase
//#define navin
//#define bearssl_rsa
//#define mbedtls_rsa

/** need to uncomment if the board you are using is MSP432P401R **/
// #define msp432p401r
// #define riscv

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/// DO NOT EDIT BELOW  //////////////////////////////////////////
#ifdef msp432p401r
#include "msp.h"
#include "rom_map.h"
#include "rom.h"
#include "systick.h"
#endif

#ifdef msp430g2553
#include "msp430.h"
#endif

#ifdef msp430fr5994
#include "msp430.h"
#endif

// #include "experiment_time.h"

#ifdef tiny_rsa
#include "rsa_test.h"
#endif
#ifdef codebase
#include "codebase/rsa.h"
#endif
#ifdef navin
#include "navin/rsa.h"
#endif
#ifdef bearssl_rsa
#include "bearssl/bearssl.h"
#include "bearssl/inner.h"
#include "bearssl/bearssl_rsa.h"
#endif
#ifdef  mbedtls_rsa
#include <rsa/mbedtls-copy/pk.h>
#endif
#include "bn.h"

/** Globals (test inputs) **/
//define the global variables here

#ifdef tiny_rsa

char resultBuffer[8192];

char public[] =
    "b7a6084628c283c8dbc3d5ce296df8e7084372e85ed2e59a602e4a5033c08b44"
    "785da139a50e044a393bfcc9fb3e979f71ec068df6fee305207f2c391fcd7f12"
    "db3ad527073a6a47a64822d22edcb7185660c06dedd2a4ccadb245ec2c16a4aa"
    "3b6a2056c0f1d87ef4c81de16f646805f4fef38e28d70e028dd38640f30a9887";

char private[] =
    "a8821a397610f5270a2f69311fe69e6a2eb4e4ecbe8538d42bc88dbb85a0f8a8"
    "bce0fc9c8e0c8c7726891a6ca2b24e21a9406ecb3885abfcfa52765b7c08ecbc"
    "ebd7e4fbe88cc668f533b62de3a0890843e96f7687bd50d3bd100ac777179b43"
    "1cb5c06be66bb515e89eed7ea9233a26dda1d89127d23ec6fbc306b98b1d8961";

char cipher[] =
    "5136258167b0c0f43e982a1948006b174f551f1d72b65acfe44dd55b98370587"
    "f87a4fa1cc6b4bf78b009c57d4c94dac2550cdb5421922a296373db63051da27"
    "4aaf595b3b359542ecba549e9ce239811ad250cc098c1518874a2be17ae082d6"
    "912b38e0b9aa4045a8fd6805f0b0c365e6075464cc394b04a8dca0d583f0ca98";

// "ABCDEFHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,." * 2
char plaintext[] =
    "4142434445464748494a4b4c4d4e4f505152535455565758595a616263646566"
    "6768696a6b6c6d6e6f707172737475767778797a313233343536373839302c2e"
    "4142434445464748494a4b4c4d4e4f505152535455565758595a616263646566"
    "6768696a6b6c6d6e6f707172737475767778797a313233343536373839302c2e";

int exponent = 65537;

#endif
#ifdef codebase

uint64_t plaintext = 54321;
uint64_t cipher = 351159;
uint64_t rsaDecrypted, rsaEncrypted;

#endif
#ifdef navin

uint64_t resultBuffer[1024];
char public[] =
        "a15f36fc7f8d188057fc51751962a5977118fa2ad4ced249c039ce36c8d1bd275273f1edd821892fa75680b1ae38749fff9268bf06b3c2af02bbdb52a0d05c2ae2384aa1002391c4b16b87caea8296cfd43757bb51373412e8fe5df2e56370505b692cf8d966e3f16bc62629874a0464a9710e4a0718637a68442e0eb1648ec5";
char private[] =
        "3f5cc8956a6bf773e598604faf71097e265d5d55560c038c0bdb66ba222e20ac80f69fc6f93769cb795440e2037b8d67898d6e6d9b6f180169fc6348d5761ac9e81f6b8879529bc07c28dc92609eb8a4d15ac4ba3168a331403c689b1e82f62518c38601d58fd628fcb7009f139fb98e61ef7a23bee4e3d50af709638c24133d";
char cipher[] =
        "1cb1c5e45e584cb1b627cac7b0de0812dac7c1d1638785a7660f6772d219f62aa0ce3e8a853abadebe0a293d76a17d321da8b1fd25ddf807ce96006f73a0aed014b990d6025c42b6c216d8553b66e724270b6dbd654d55e368edeacbc8da30f0cbe5ccbb72a3fe44d29543a5bbb5255a404234ce53bf70f52a78170685a6e391";
uint8_t plaintext[] = {5, 4, 3, 2, 1};

#endif


/** Initialization for different RSA implementations **/

#ifdef tiny_rsa
// Nothing needs to be done.
#endif
#ifdef codebase

// Values based on 64-bit math (huge_t = uint64_t)
// which will result in more secure encryption, but also
// increases the size of the encrypted text

rsaPubKey_t publicKey = {21, 16484947};
rsaPriKey_t privateKey = {15689981, 16484947};

//publicKey.e = 21;
//publicKey.n = 16484947;
//privateKey.d = 15689981;
//privateKey.n = 16484947;

#endif
#ifdef navin
// Nothing needs to be done.
#endif


void test_encrypt()
{
#ifdef tiny_rsa
    rsa1024_encrypt(public, private, resultBuffer, exponent, plaintext);
#endif
#ifdef codebase
    rsaEncrypt(plaintext, &rsaEncrypted, publicKey);
#endif
#ifdef navin
    uint64_t e[18] = {0};
    e[0] = 65537;

    rsa1024(resultBuffer, plaintext, public, e);
#endif
    // printf("plaintext %d\r\n", plaintext);
    // printf("resultBuffer %s\r\n", resultBuffer);
}

void test_decrypt()
{
#ifdef tiny_rsa
    rsa1024_decrypt(public, private, resultBuffer, cipher);
#endif
#ifdef codebase
    rsaDecrypt(cipher, &rsaDecrypted, privateKey);
#endif
#ifdef navin
    uint64_t e[18] = {0};
    e[0] = 65537;

    rsa1024(resultBuffer, ciphertext, private, e);
#endif

}

int check_encrypt()
{
#if defined(tiny_rsa)
    return memcmp((char*) cipher, (char*) resultBuffer, sizeof(cipher));
#elif defined(codebase)
    return (cipher == rsaEncrypted);
#elif defined(navin)
    return memcmp((char*) cipher, (char*) resultBuffer, sizeof(cipher));
#endif
}

int check_decrypt()
{
#if defined(tiny_rsa)
    return memcmp((char*) plaintext, (char*) resultBuffer, strlen(plaintext));
#elif defined(codebase)
    return (plaintext == rsaDecrypted);
#elif defined(navin)
    return memcmp((char*) plaintext, (char*) resultBuffer, sizeof(plaintext));
#endif
}

int main(void)
{
#ifdef msp432p401r
    /** Initialize the board **/
    board_init();

    /** Starting the timer to measure elapsed time **/
    startTimer();
#endif

    /** test rsa **/
    test_encrypt();
    volatile unsigned int verify_encrypt = check_encrypt();
    printf("Encryption %s\r\n\r\n", verify_encrypt ? "failed" : "successful");

    test_decrypt();

    /** Check the result to see whether RSA algorithm is correctly working or not **/
    volatile unsigned int verify_decrypt = check_decrypt();
    printf("Decryption %s\r\n", verify_decrypt ? "failed" : "successful");

#ifdef msp432p401r
    volatile unsigned int elapsed = getElapsedTime();
#endif

    /** Print the result **/

    // while (1);
    return 0;
}
