//
// Created by 廖哲賢 on 2021/10/12.
//

#ifndef CURVE_H
#define CURVE_H

/* BSD Support Curve. Set to 0 to remove that curve. */
#ifndef uECC_SUPPORTS_secp160r1
#define uECC_SUPPORTS_secp160r1 0
#endif
#ifndef uECC_SUPPORTS_secp192r1
#define uECC_SUPPORTS_secp192r1 0
#endif
#ifndef uECC_SUPPORTS_secp224r1
#define uECC_SUPPORTS_secp224r1 0
#endif
#ifndef uECC_SUPPORTS_secp256r1
#define uECC_SUPPORTS_secp256r1 0
#endif
#ifndef uECC_SUPPORTS_secp256k1
#define uECC_SUPPORTS_secp256k1 1
#endif

/*Tiny ECC Support Curve*/
#define NIST_B163 1
#define NIST_B233 3
#define NIST_B283 5
#define NIST_B409 7
#define NIST_B571 9

/* What is the default curve to use? */
#ifndef ECC_CURVE
#define ECC_CURVE NIST_B233 // change this name to switch curve  ex NIST_B163 -> NIST_B233
#endif

/* MbedTLS ECC */
// Size of buffer used to store the public keys exchanged between the client and sever
// Buffer size should be the following:
// Curve    Public Key Buffer Size
// -----    ----------------------
// 25519    32
// SECP384  48
// SECP521  66
#define BUF_BYTES 32 // Safe to use the largest buffer size

#define CURVE25519 1
#define SECP384R1 0
#define SECP521R1 0

#if CURVE25519
#define ELLIPTIC_CURVE MBEDTLS_ECP_DP_CURVE25519
#elif SECP384R1
#define ELLIPTIC_CURVE MBEDTLS_ECP_DP_SECP384R1
#elif SECP521R1
#define ELLIPTIC_CURVE MBEDTLS_ECP_DP_SECP521R1
#endif

#endif // AES_TEST_CURVE_H
