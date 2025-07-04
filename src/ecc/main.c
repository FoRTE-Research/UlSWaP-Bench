#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "curve.h"
#include "ecdh.h"
#include "common.h"

#define ECC_PUB_SIZE ECC_PUB_KEY_SIZE
#define ECC_PRV_SIZE ECC_PRV_KEY_SIZE

uint8_t sec_a[ECC_PUB_SIZE];
uint8_t sec_b[ECC_PUB_SIZE];

#if (ECC_CURVE == NIST_K163) || (ECC_CURVE == NIST_B163)
const uint8_t prv_a[ECC_PRV_SIZE] = {0x2F, 0x46, 0x53, 0xB8, 0x26, 0x67, 0x5E, 0x49, 0x72, 0x6B, 0x53, 0xE4,
                                     0x99, 0x55, 0x09, 0x99, 0xBE, 0x39, 0x47, 0x60, 0x02, 0x00, 0x00, 0x00};
const uint8_t prv_b[ECC_PRV_SIZE] = {0x8F, 0x69, 0x8F, 0x5A, 0x1D, 0xC2, 0x2D, 0x84, 0x3E, 0xC0, 0x0C, 0xAF,
                                     0x89, 0x04, 0x52, 0x39, 0x97, 0x7D, 0x51, 0x4F, 0x02, 0x00, 0x00, 0x00};
const uint8_t pub_a[ECC_PUB_SIZE] = {0x55, 0x62, 0x95, 0xC4, 0xE6, 0xCA, 0x69, 0x7A, 0x8D, 0x82, 0xBF, 0xCC,
                                     0xDA, 0xD2, 0x22, 0xCA, 0x2A, 0xA2, 0xA3, 0x5D, 0x02, 0x00, 0x00, 0x00,
                                     0xB5, 0x58, 0xD9, 0x07, 0x81, 0x35, 0xB5, 0x19, 0xDD, 0x1A, 0xFE, 0x4A,
                                     0x0F, 0xA2, 0x83, 0xB6, 0x5D, 0xC1, 0xAC, 0x67, 0x03, 0x00, 0x00, 0x00};
const uint8_t pub_b[ECC_PUB_SIZE] = {0x55, 0x04, 0xB5, 0x37, 0x9A, 0xFC, 0x4A, 0x86, 0xE4, 0xF6, 0xDD, 0xFA,
                                     0x89, 0xBA, 0x60, 0x49, 0xF1, 0xE7, 0xE9, 0x0F, 0x03, 0x00, 0x00, 0x00,
                                     0x14, 0xB8, 0xA3, 0xE0, 0xE7, 0xF0, 0xC3, 0xED, 0x15, 0x54, 0x68, 0xD9,
                                     0x2D, 0x7C, 0x56, 0xEF, 0x51, 0x7F, 0x44, 0x2F, 0x07, 0x00, 0x00, 0x00};
#elif (ECC_CURVE == NIST_K233) || (ECC_CURVE == NIST_B233)
const uint8_t prv_a[ECC_PRV_SIZE] = {0x2F, 0x46, 0x53, 0xB8, 0x26, 0x67, 0x5E, 0x49, 0x72, 0x6B, 0x53, 0xE4,
                                     0x99, 0x55, 0x09, 0x99, 0xBE, 0x39, 0x47, 0x60, 0xF2, 0x91, 0x87, 0x1F,
                                     0x8F, 0x69, 0x8F, 0x5A, 0x1D, 0x00, 0x00, 0x00};
const uint8_t prv_b[ECC_PRV_SIZE] = {0x3E, 0xC0, 0x0C, 0xAF, 0x89, 0x04, 0x52, 0x39, 0x97, 0x7D, 0x51, 0x4F,
                                     0xDA, 0x58, 0xAE, 0x16, 0xA9, 0x63, 0x74, 0x67, 0x94, 0x93, 0xB1, 0xF7,
                                     0x66, 0x7B, 0x29, 0x1C, 0xCE, 0x00, 0x00, 0x00};
const uint8_t pub_a[ECC_PUB_SIZE] = {0xAF, 0x08, 0xF4, 0x95, 0x0A, 0x27, 0x5F, 0x50, 0x96, 0x38, 0x69, 0x45,
                                     0x9D, 0x40, 0xDF, 0x2B, 0x25, 0xA9, 0x63, 0x35, 0xD4, 0xAC, 0x61, 0x69,
                                     0x2D, 0x3D, 0x1F, 0xE2, 0x38, 0x00, 0x00, 0x00, 0x31, 0xAE, 0x94, 0xD6,
                                     0x9C, 0x7D, 0xA4, 0x44, 0x67, 0xC7, 0x1E, 0x66, 0xD6, 0x36, 0x92, 0x6F,
                                     0x8F, 0x42, 0xB9, 0xBF, 0x71, 0xB4, 0x4F, 0x5B, 0x41, 0xAD, 0xAA, 0x86,
                                     0x82, 0x01, 0x00, 0x00};
const uint8_t pub_b[ECC_PUB_SIZE] = {0x70, 0xF7, 0x8E, 0xB9, 0x4E, 0x59, 0x98, 0xF8, 0xB3, 0x3A, 0x48, 0xE2,
                                     0x3C, 0x12, 0x8A, 0x9C, 0x06, 0xE9, 0x34, 0xD6, 0xBC, 0x32, 0xAD, 0xDC,
                                     0xFD, 0x2D, 0x03, 0xA3, 0xC7, 0x00, 0x00, 0x00, 0x77, 0xAF, 0xA1, 0xA4,
                                     0x31, 0x68, 0xBA, 0xF6, 0x78, 0x29, 0xCB, 0xA6, 0xDC, 0xA3, 0x52, 0x63,
                                     0xC0, 0xAB, 0x59, 0x53, 0x0C, 0x74, 0xEC, 0x46, 0x3E, 0x20, 0xDF, 0x38,
                                     0x62, 0x00, 0x00, 0x00};
#elif (ECC_CURVE == NIST_K283) || (ECC_CURVE == NIST_B283)
const uint8_t prv_a[ECC_PRV_SIZE] = {0x2F, 0x46, 0x53, 0xB8, 0x26, 0x67, 0x5E, 0x49, 0x72, 0x6B, 0x53, 0xE4,
                                     0x99, 0x55, 0x09, 0x99, 0xBE, 0x39, 0x47, 0x60, 0xF2, 0x91, 0x87, 0x1F,
                                     0x8F, 0x69, 0x8F, 0x5A, 0x1D, 0xC2, 0x2D, 0x84, 0x3E, 0xC0, 0x0C, 0x01};
const uint8_t prv_b[ECC_PRV_SIZE] = {0x89, 0x04, 0x52, 0x39, 0x97, 0x7D, 0x51, 0x4F, 0xDA, 0x58, 0xAE, 0x16,
                                     0xA9, 0x63, 0x74, 0x67, 0x94, 0x93, 0xB1, 0xF7, 0x66, 0x7B, 0x29, 0x1C,
                                     0xCE, 0x0E, 0x9C, 0x30, 0xB2, 0x51, 0xB2, 0x5F, 0x52, 0xCD, 0x6D, 0x01};
const uint8_t pub_a[ECC_PUB_SIZE] = {0x69, 0xA6, 0x4E, 0xD8, 0x31, 0x16, 0x63, 0xD5, 0x34, 0xAA, 0x31, 0xC6,
                                     0xD1, 0xF0, 0xDF, 0xB8, 0x54, 0x3D, 0x33, 0x61, 0x81, 0xAB, 0x05, 0x88,
                                     0x54, 0x91, 0x8A, 0x55, 0x82, 0x92, 0x5A, 0x9D, 0xB6, 0x33, 0x33, 0x06,
                                     0x73, 0xC7, 0x5F, 0xD6, 0xD2, 0x52, 0xDA, 0x22, 0x31, 0x90, 0x2F, 0xE0,
                                     0xF6, 0x49, 0x72, 0x25, 0x54, 0xBA, 0xAC, 0x88, 0xE0, 0x0A, 0xA5, 0x3B,
                                     0x17, 0x9F, 0x0A, 0x49, 0xBF, 0x0B, 0x93, 0x10, 0x77, 0x6D, 0xF7, 0x06};
const uint8_t pub_b[ECC_PUB_SIZE] = {0x8C, 0x90, 0x69, 0x3D, 0x53, 0xFE, 0x52, 0xA6, 0x1E, 0xDF, 0x89, 0x3B,
                                     0x65, 0x60, 0xE6, 0x25, 0x1E, 0x8D, 0x64, 0x97, 0xAE, 0x52, 0x2E, 0xCD,
                                     0x45, 0x3D, 0x2A, 0x05, 0x50, 0x5A, 0x69, 0x58, 0x18, 0xFC, 0x03, 0x05,
                                     0xBC, 0x72, 0xE8, 0x6A, 0x11, 0x4C, 0xC5, 0x08, 0x2D, 0x4E, 0x03, 0x86,
                                     0xA3, 0x26, 0xCE, 0xC9, 0x9A, 0xC9, 0x13, 0x55, 0x5E, 0xD1, 0x10, 0x2B,
                                     0xD7, 0x15, 0xFF, 0x7C, 0xC6, 0x2B, 0xAB, 0x2F, 0xA4, 0x82, 0xE9, 0x03};
#elif (ECC_CURVE == NIST_K409) || (ECC_CURVE == NIST_B409)
const uint8_t prv_a[ECC_PRV_SIZE] = {0x2F, 0x46, 0x53, 0xB8, 0x26, 0x67, 0x5E, 0x49, 0x72, 0x6B, 0x53, 0xE4,
                                     0x99, 0x55, 0x09, 0x99, 0xBE, 0x39, 0x47, 0x60, 0xF2, 0x91, 0x87, 0x1F,
                                     0x8F, 0x69, 0x8F, 0x5A, 0x1D, 0xC2, 0x2D, 0x84, 0x3E, 0xC0, 0x0C, 0xAF,
                                     0x89, 0x04, 0x52, 0x39, 0x97, 0x7D, 0x51, 0x4F, 0xDA, 0x58, 0xAE, 0x16,
                                     0xA9, 0x63, 0x74, 0x00};
const uint8_t prv_b[ECC_PRV_SIZE] = {0x94, 0x93, 0xB1, 0xF7, 0x66, 0x7B, 0x29, 0x1C, 0xCE, 0x0E, 0x9C, 0x30,
                                     0xB2, 0x51, 0xB2, 0x5F, 0x52, 0xCD, 0x6D, 0xD3, 0x74, 0x53, 0x07, 0x49,
                                     0xFC, 0xD3, 0x61, 0xE7, 0x31, 0xED, 0x4E, 0x92, 0xFF, 0x2D, 0x12, 0xF7,
                                     0x2C, 0x01, 0x29, 0x2E, 0xE4, 0x60, 0x0D, 0x79, 0x7E, 0x1E, 0xCE, 0x93,
                                     0x7F, 0x90, 0xD7, 0x00};
const uint8_t pub_a[ECC_PUB_SIZE] = {0xA3, 0x47, 0x34, 0x12, 0x0E, 0x47, 0x9E, 0xE7, 0x59, 0x50, 0x0D, 0xA8,
                                     0xA0, 0xF9, 0xEB, 0x28, 0x3E, 0xE2, 0x4B, 0xFC, 0x15, 0x73, 0x4F, 0xCC,
                                     0xBB, 0x14, 0x78, 0x97, 0x72, 0xE8, 0x17, 0x43, 0x2D, 0x83, 0xBD, 0x67,
                                     0x91, 0x0A, 0x7B, 0xCC, 0x45, 0xE9, 0x73, 0x03, 0x57, 0xDD, 0xE7, 0x9F,
                                     0x1F, 0x25, 0xBE, 0x00, 0x4F, 0x52, 0x5E, 0x0E, 0x16, 0xC4, 0xBC, 0xAF,
                                     0x29, 0x01, 0x7E, 0xCA, 0x89, 0x07, 0xE9, 0xCC, 0x66, 0xFA, 0xA2, 0x56,
                                     0x31, 0xED, 0xCE, 0x49, 0xF1, 0x62, 0xF0, 0x3F, 0x52, 0x53, 0x39, 0x9A,
                                     0x0D, 0x7A, 0x10, 0x4E, 0x24, 0xCB, 0x1A, 0x3F, 0x54, 0xE7, 0x60, 0xFF,
                                     0x27, 0x96, 0xB1, 0x05, 0x79, 0x9E, 0x37, 0x01};
const uint8_t pub_b[ECC_PUB_SIZE] = {0x5F, 0x47, 0xF0, 0xA4, 0xD3, 0x47, 0xED, 0xCF, 0xD9, 0x3E, 0x11, 0x91,
                                     0x09, 0x91, 0x31, 0xA6, 0xBA, 0x13, 0x62, 0xC6, 0xF6, 0x14, 0x82, 0xD7,
                                     0x3D, 0x0A, 0xA1, 0x5B, 0x42, 0xEA, 0xBA, 0x83, 0xE1, 0x19, 0x3B, 0xE2,
                                     0xEB, 0xB3, 0xC4, 0x01, 0xA4, 0xB3, 0x03, 0x1C, 0xEC, 0xFD, 0xF0, 0x71,
                                     0x98, 0xA4, 0x21, 0x01, 0xD4, 0x54, 0x78, 0xEA, 0x70, 0x50, 0x6B, 0xB6,
                                     0x5E, 0x6B, 0x85, 0x0F, 0x66, 0x78, 0x63, 0x99, 0x42, 0x22, 0x93, 0xCC,
                                     0x89, 0x77, 0xEB, 0x9E, 0xD9, 0x67, 0x03, 0x0A, 0xE8, 0xFC, 0x5B, 0xA3,
                                     0x42, 0x14, 0x20, 0xAE, 0x39, 0x08, 0xE5, 0xFE, 0x2A, 0x39, 0xD7, 0xC8,
                                     0x41, 0xC7, 0xFA, 0x26, 0x5B, 0xF5, 0xDE, 0x00};
#elif (ECC_CURVE == NIST_K571) || (ECC_CURVE == NIST_B571)
const uint8_t prv_a[ECC_PRV_SIZE] = {0x2F, 0x46, 0x53, 0xB8, 0x26, 0x67, 0x5E, 0x49, 0x72, 0x6B, 0x53, 0xE4,
                                     0x99, 0x55, 0x09, 0x99, 0xBE, 0x39, 0x47, 0x60, 0xF2, 0x91, 0x87, 0x1F,
                                     0x8F, 0x69, 0x8F, 0x5A, 0x1D, 0xC2, 0x2D, 0x84, 0x3E, 0xC0, 0x0C, 0xAF,
                                     0x89, 0x04, 0x52, 0x39, 0x97, 0x7D, 0x51, 0x4F, 0xDA, 0x58, 0xAE, 0x16,
                                     0xA9, 0x63, 0x74, 0x67, 0x94, 0x93, 0xB1, 0xF7, 0x66, 0x7B, 0x29, 0x1C,
                                     0xCE, 0x0E, 0x9C, 0x30, 0xB2, 0x51, 0xB2, 0x5F, 0x52, 0xCD, 0x6D, 0x01};
const uint8_t prv_b[ECC_PRV_SIZE] = {0x74, 0x53, 0x07, 0x49, 0xFC, 0xD3, 0x61, 0xE7, 0x31, 0xED, 0x4E, 0x92,
                                     0xFF, 0x2D, 0x12, 0xF7, 0x2C, 0x01, 0x29, 0x2E, 0xE4, 0x60, 0x0D, 0x79,
                                     0x7E, 0x1E, 0xCE, 0x93, 0x7F, 0x90, 0xD7, 0x77, 0x93, 0xDA, 0x7C, 0x7B,
                                     0xA5, 0x66, 0x70, 0xF2, 0x63, 0xD6, 0xA2, 0x42, 0x1F, 0x0A, 0x9E, 0x31,
                                     0x75, 0xB5, 0x76, 0x66, 0x71, 0x4C, 0x93, 0xD1, 0xCC, 0x26, 0x9F, 0xE6,
                                     0xB8, 0x38, 0x7B, 0x4E, 0x2D, 0xB4, 0x18, 0x69, 0xC5, 0x77, 0x9C, 0x01};
const uint8_t pub_a[ECC_PUB_SIZE] = {0xCA, 0xF3, 0x7F, 0x9F, 0x5C, 0x8F, 0xD5, 0xF0, 0x51, 0xB3, 0x8D, 0x60,
                                     0xB7, 0x97, 0x14, 0x78, 0xBC, 0xBC, 0x09, 0xD5, 0x13, 0xB8, 0x23, 0x0B,
                                     0xDB, 0x43, 0xC3, 0x97, 0xD2, 0xDE, 0x95, 0x84, 0x91, 0xBE, 0xA4, 0x03,
                                     0x4D, 0xBC, 0xF3, 0x73, 0x8C, 0x87, 0xAD, 0x68, 0x05, 0x84, 0x8A, 0xC9,
                                     0x83, 0x1B, 0xDB, 0x52, 0x67, 0xB5, 0xD2, 0x6D, 0x63, 0x95, 0xDF, 0xA6,
                                     0x9F, 0x77, 0x99, 0x1C, 0x7F, 0x70, 0x00, 0x7A, 0xF5, 0x6D, 0x8B, 0x01,
                                     0xDA, 0x98, 0x2C, 0x0F, 0x86, 0xCD, 0x6F, 0x3E, 0xE2, 0x63, 0x5A, 0x16,
                                     0x93, 0x98, 0x74, 0x83, 0x40, 0x76, 0xD5, 0x90, 0x82, 0x1D, 0xD9, 0x64,
                                     0xC1, 0xAD, 0x2D, 0xAB, 0x26, 0xD9, 0x28, 0x7C, 0xD4, 0x79, 0x61, 0xF4,
                                     0xA8, 0x5B, 0x5E, 0xA1, 0xE7, 0x96, 0xBC, 0x5B, 0x6A, 0x74, 0xC3, 0x54,
                                     0x12, 0xD4, 0xB1, 0x52, 0xE1, 0xD5, 0xFC, 0x77, 0x24, 0x36, 0xD4, 0xA5,
                                     0x6A, 0xFF, 0x78, 0x38, 0x86, 0x1F, 0xCC, 0x36, 0x86, 0xFD, 0xA1, 0x01};
const uint8_t pub_b[ECC_PUB_SIZE] = {0xE4, 0x7C, 0x54, 0x27, 0x16, 0x45, 0xA5, 0xB9, 0x98, 0x91, 0xCA, 0x7B,
                                     0x19, 0x3B, 0xC7, 0xB5, 0x25, 0x7E, 0x50, 0x02, 0x16, 0x77, 0x7C, 0x82,
                                     0xF3, 0x17, 0x33, 0x4E, 0x12, 0xC4, 0x84, 0x5F, 0xE2, 0x8D, 0x12, 0x57,
                                     0xE2, 0x7A, 0xBE, 0x12, 0x6A, 0x84, 0x56, 0x02, 0x53, 0xE8, 0xAC, 0x63,
                                     0x74, 0xAF, 0x78, 0x64, 0xDF, 0xC3, 0x24, 0xFA, 0xF6, 0x00, 0x14, 0x33,
                                     0x13, 0xCC, 0x8E, 0x6A, 0xC7, 0x45, 0xA7, 0xD9, 0xBB, 0xCF, 0x49, 0x07,
                                     0xA6, 0xDE, 0xC0, 0x44, 0xF2, 0x38, 0x9D, 0xA1, 0x09, 0xB6, 0x43, 0x97,
                                     0x45, 0x15, 0x1B, 0x84, 0x28, 0xD6, 0xF1, 0x34, 0xB2, 0xD8, 0xD0, 0xB6,
                                     0xDB, 0xE2, 0x91, 0x4B, 0x99, 0x8D, 0xBB, 0xFC, 0x75, 0x1A, 0xCF, 0x9A,
                                     0x02, 0x07, 0x29, 0x63, 0x3F, 0xD0, 0x31, 0x89, 0x7B, 0x3C, 0xBE, 0xDD,
                                     0x18, 0x3B, 0x98, 0x89, 0xB4, 0xE6, 0x72, 0x71, 0xF0, 0x6D, 0xA3, 0xAA,
                                     0xA5, 0xB1, 0xF2, 0xF6, 0x87, 0x21, 0xD2, 0x4F, 0x8F, 0x6B, 0xA9, 0x03};
#endif

#define MAX_NUMS_PER_LINE 32
void print_hex(const char* name, const uint8_t *data, size_t len)
{
    if (name)
    {
        printf("%s:\r\n", name);
    }
    for (size_t i = 0; i < len; ++i)
    {
        printf("%02x ", data[i]);
        if ((i + 1) % MAX_NUMS_PER_LINE == 0)
        {
            printf("\r\n");
        }
    }
    printf("\r\n");
}

int check_result()
{
    return memcmp(sec_a, sec_b, ECC_PUB_SIZE);
}

benchmark_hash_t benchmark_main(void)
{
    benchmark_hash_t benchmark_hash_ret = 0;

    ecdh_shared_secret(prv_a, pub_b, sec_a);
    print_hex("Shared Secret A", sec_a, ECC_PUB_SIZE);

    ecdh_shared_secret(prv_b, pub_a, sec_b);
    print_hex("Shared Secret B", sec_b, ECC_PUB_SIZE);

    return benchmark_hash_ret;
}
