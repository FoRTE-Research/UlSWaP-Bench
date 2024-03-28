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
    // "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ca1";
    // "00b261a64fd5b9977efcc821a11cc8fb8f99aedb05b9055acb41b2f7cb0ad5e4e4c3471e8e3fe27d1b9a44ebc415294d608dd25cf74d61a794cc1c4642a0dd5ed529973401b383dd19678cee1bc46b855d95c0202c745f35f6f8d23b7c9d789d80337a5e01c6145a7710078bbcfbf3a0066c74f3ddb4d61ec99644f6f31b9c380d";
    "ed6af0e36dd8f1d935913da1b6142aefad1ab8a448b8577a8afe9d477bd59cdcb0daa0f75bf7ca54e743006e19bf60ec9c0aa7005a54298c54becdf34b29753f";

char private[] =
    // "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000019d";
    // "76819f2ebcab5eb53394759b0677938074361135fd144c9c6a1a4ebaea88ed56f950bf40fbba29297454c358f2420d30cf276931b68be0fc5e998e27e79d09c971907feae9342ef5f93fc1b7f1fef735b899df569807e3cac59dd0159d78fca6f4e9f8af57d2b5ec5c2ab83a0d5c760c12f63e3cd40c4c2958613ec3198d0d41";
    "8b12d7e50195d1a60141012f572f5d2f64e180438beda020c286ffaebe15957a78a48de536e06498fb42ee38cfcb79a90afa646cf1e58d606f8db0b16b46ebe1";
// const char public[] =
//         "a15f36fc7f8d188057fc51751962a5977118fa2ad4ced249c039ce36c8d1bd275273f1edd821892fa75680b1ae38749fff9268bf06b3c2af02bbdb52a0d05c2ae2384aa1002391c4b16b87caea8296cfd43757bb51373412e8fe5df2e56370505b692cf8d966e3f16bc62629874a0464a9710e4a0718637a68442e0eb1648ec5";
// const char private[] =
//         "3f5cc8956a6bf773e598604faf71097e265d5d55560c038c0bdb66ba222e20ac80f69fc6f93769cb795440e2037b8d67898d6e6d9b6f180169fc6348d5761ac9e81f6b8879529bc07c28dc92609eb8a4d15ac4ba3168a331403c689b1e82f62518c38601d58fd628fcb7009f139fb98e61ef7a23bee4e3d50af709638c24133d";
char cipher[] =
    "21d7bfc73cdc62dfbb6ba7b28b84bd58d1689171bcdab8391d6c1527faf58bc018816f874ff80ac7e6c71b17fc7cfa9f7e625ecf9bf309d2773fcc34a5f5f9ae";
    // "dc784361bd820b285a3aab973dc49b66c549ed6399d7842a59a36dace006bbf5f4b3f7e0c2fbece2daa56de6dbc77b7b159eb0ef1938c1eb8ea3b3194aec315b";
    // "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009b57eb2480e24284638729e7272e546178b47ff7171bc8cd211976157cc7e3aa7237d727ab925953e0a6f3602d0c1ea773538c38795e8b0ce8e3a11f1f8937c9";
    // "7480a6fb461120c9b12a91e9f84a891236d8ecbcfb6ee6ad25664bac621fefd9d7981467f6b3bda57cdf8535d61940b8f75aac506b308a20e4b1e672a214319b15d23278addb073d91b6e5ced99168b45cb77050703ed501b2ff7e19abbf21f63cedd57c5cab099fe06bdde7dc7f14b2ef56f1ac01e26f621c40f6a110b0d431";
    // "1fd00be7d1ef53939e471b698d966815265cab71373cab3b5faaee0d6d4fceb0e6947e83deb485116dbae4fcda81cc30b6936323d8de755d4d0d9b02009a2ce241cb98627dc7ad0a47af80526ac3033bc92acb12d05541f7913fbcf7278af77f6c937eb6d8748955e6ebfd80f98e824bf2a84eb334d9f3891292429df5f02b83";
    // "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000080b89cc5ce40496de7f2f92df9b5f2658ee91c3b46d0a9a8c8bf7260782132df8c9c27e5834ffa70b5feb77396dedbcd547b7bcfb10f8e75ff004c1cabd7db27";
//         "1cb1c5e45e584cb1b627cac7b0de0812dac7c1d1638785a7660f6772d219f62aa0ce3e8a853abadebe0a293d76a17d321da8b1fd25ddf807ce96006f73a0aed014b990d6025c42b6c216d8553b66e724270b6dbd654d55e368edeacbc8da30f0cbe5ccbb72a3fe44d29543a5bbb5255a404234ce53bf70f52a78170685a6e391";
// char cipher[] =
//     "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ae6";

// "ABCDEFHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,."
char plaintext[] = "4142434445464748494a4b4c4d4e4f505152535455565758595a6162636465666768696a6b6c6d6e6f707172737475767778797a313233343536373839302c2e";
// char plaintext[] = "4142434445464748494a4b4c4d4e4f505152535455565758595a6162636465666768696a6b6c6d6e6f707172737475767778797a31323334353637";

int exponent = 65537;
// char plaintext[] = "{5, 4, 3, 2, 1}";

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
printf("plaintext %s\r\n", plaintext);
printf("resultBuffer %s\r\n", resultBuffer);
    return memcmp((char*) plaintext, (char*) resultBuffer, strlen(plaintext));
#elif defined(codebase)
    return (plaintext == rsaDecrypted);
#elif defined(navin)
    return memcmp((char*) plaintext, (char*) resultBuffer, sizeof(plaintext));
#endif
}

int main(void)
{
    // struct bn a;
    // bignum_init(&a);
    // bignum_from_int(&a, 54321);
    // bignum_dump(&a);
    // return 0;



#ifdef msp432p401r
    /** Initialize the board **/
    board_init();

    /** Starting the timer to measure elapsed time **/
    startTimer();
#endif

    /** test rsa **/
   test_encrypt();
   printf("Encryption complete\r\n");
//    printf("plaintext %d\r\n", plaintext);
//     printf("cipher %s\r\n", cipher);
//     printf("resultBuffer %s\r\n", resultBuffer);
    volatile unsigned int verify_encrypt = check_encrypt();
    printf("Encryption %s\r\n", verify_encrypt ? "failed" : "successful");


    test_decrypt();
    printf("Decryption complete\r\n");

//    printf("plaintext %d\r\n", plaintext);
//     printf("cipher %s\r\n", cipher);
//     printf("resultBuffer\r\n");

//     // print result buffer
//     for (int i = 0; i < sizeof(resultBuffer); i++) {
//         printf("%c", resultBuffer[i]);
//     }
//     printf("\r\n");
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
