//
// Created by willi on 2022/2/22.
// This file contains functions that have been extracted from rsa.c of tiny RSA.
// The function names have been modified from the original rsa.c.
//

#include <stdio.h>
#include "bn.h"

/* O(log n) */
// res = a^b mod n
void pow_mod_faster(struct bn* a, struct bn* b, struct bn* n, struct bn* res)
{
    bignum_from_int(res, 1); /* r = 1 */

    struct bn tmpa;
    struct bn tmpb;
    struct bn tmp;
    bignum_assign(&tmpa, a);
    bignum_assign(&tmpb, b);

    while (1)
    {
        if (tmpb.array[0] & 1)     /* if (b % 2) */
        {
            bignum_mul(res, &tmpa, &tmp);  /*   r = r * a % m */
            bignum_mod(&tmp, n, res);
        }
        bignum_rshift(&tmpb, &tmp, 1); /* b /= 2 */
        bignum_assign(&tmpb, &tmp);

        if (bignum_is_zero(&tmpb))
            break;

        bignum_mul(&tmpa, &tmpa, &tmp);
        bignum_mod(&tmp, n, &tmpa);
    }
}

void rsa1024_encrypt(char *public, char *private, char *buff, int exponent, char *plaintext_hex)
{
    printf("Encrypting...\r\n");

    char buf[8192];

    struct bn n; /* public  key */
    struct bn d; /* private key */
    struct bn e; /* public exponent */
    struct bn m; /* clear text message */
    struct bn c; /* cipher text */

    bignum_init(&n);
    bignum_init(&d);
    bignum_init(&e);
    bignum_init(&m);
    bignum_init(&c);

    bignum_from_string(&n, public,  256);
    bignum_from_string(&d, private, 256);
    bignum_from_int(&e, exponent);
    bignum_init(&m);
    bignum_init(&c);

    printf("Public key (bignum):\r\n");
    bignum_dump(&n, 32);
    printf("Exponent (bignum):\r\n");
    bignum_dump(&e, 32);

    bignum_from_string(&m, plaintext_hex, 256);
    bignum_to_string(&m, buf, sizeof(buf));

    printf("Plaintext (bignum):\r\n");
    bignum_dump(&m, 32);

    /** Encrypting **/
    pow_mod_faster(&m, &e, &n, &c);
    bignum_to_string(&c, buf, sizeof(buf));

    printf("Ciphertext (bignum):\r\n");
    bignum_dump(&c, 32);

    for(int i = 0; i < sizeof(buf); i++) {
        buff[i] = buf[i];
    }

}

void rsa1024_decrypt(char *public, char *private, char *buff, char *cipher)
{
    printf("Decrypting...\r\n");

    char buf[8192];

    struct bn n; /* public  key */
    struct bn d; /* private key */
    struct bn m; /* clear text message */
    struct bn c; /* cipher text */

    //int len_pub = strlen(public);
    //int len_prv = strlen(private);

    bignum_init(&n);
    bignum_init(&d);
    bignum_init(&m);
    bignum_init(&c);

    bignum_from_string(&n, public,  256);
    bignum_from_string(&d, private, 256);
    bignum_from_string(&c, cipher, 256);

    printf("Private key (bignum):\r\n");
    bignum_dump(&d, 32);

    pow_mod_faster(&c, &d, &n, &m);

    printf("Decrypted plaintext (bignum):\r\n");
    bignum_dump(&m, 32);

    bignum_to_string(&m, buf, sizeof(buf));

    for(int i = 0; i < sizeof(buf); i++) {
        buff[i] = buf[i];
    }
}
