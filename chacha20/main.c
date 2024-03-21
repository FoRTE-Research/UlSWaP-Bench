/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

//#define riscv
//#define saml11
//#define apollo3
//#define msp430f5529
//#define msp430fr5994
// #define msp432p401r
//#define adafruitm0express

//#define portable8439
//#define chacha_avr
#define ChaCha20_rfc7539
// #define mbedtls

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef riscv
#include <metal/cpu.h>
#include <metal/interrupt.h>
#include <metal/timer.h>
#endif

#ifdef saml11
#include <saml11e16a.h>
#include "atmel_start.h"
#include <hal_gpio.h>
#include <hal_delay.h>
//115200 Baud rate
static const uint8_t welcoming_str[] = "Time: \r\n";
/* For measurement method; if TIMER is used then just set uncomment this line*/
#define TIMER
#endif

#ifdef apollo3
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
#include <assert.h>
#include "D:\AmbiqSuiteSDK-master\mcu\apollo3\am_mcu_apollo.h"
#include "D:\AmbiqSuiteSDK-master\boards\apollo3_evb\bsp\am_bsp.h"
#include "D:\AmbiqSuiteSDK-master\boards\apollo3_evb\bsp\am_bsp_pins.h"
#include "D:\AmbiqSuiteSDK-master\utils\am_util.h"
#include "D:\AmbiqSuiteSDK-master\utils\am_util_stdio.h"
#include "D:\AmbiqSuiteSDK-master\mcu\apollo3\hal\am_hal_uart.h"
#include "D:\AmbiqSuiteSDK-master\mcu\apollo3\hal\am_hal_pin.h"
#include "D:\AmbiqSuiteSDK-master\mcu\apollo3\hal\am_hal_stimer.h"
#include "D:\AmbiqSuiteSDK-master\mcu\apollo3\hal\am_hal_status.h"
#include "D:\AmbiqSuiteSDK-master\mcu\apollo3\hal\am_hal_sysctrl.h"
#endif

#ifdef msp432p401r
#define UART
#include "msp432.h"
#include "rom_map.h"
#include "rom.h"
#include "systick.h"
#ifdef UART
#include "driverlib.h"
#endif
#endif

#ifdef msp430fr5994
#define UART
#include "msp430.h"
#ifdef UART
#include "driverlib.h"
#endif
#endif

#ifdef msp430f5529
#define UART
#include "msp430.h"
#ifdef UART
#include "driverlib.h"
#endif
#endif

#if defined riscv || defined saml11 || defined msp432p401r || defined msp430fr5994 || defined msp430f5529
#include "experiment_time.h"
#endif

#ifdef apollo3
#include "D:\AmbiqSuiteSDK-master\boards\apollo3_evb\examples\apollo3_chacha\keil\experiment_time.h"
#endif

#ifdef portable8439
#include "portable8439/chacha-portable/chacha-portable.h"
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#endif

#ifdef chacha_avr
#include "chacha20_avr\chacha.h"
#endif

#ifdef ChaCha20_rfc7539
#define CHACHA20_IMPLEMENTATION
#define CHACHA20_NO_UNDEF
#include "ChaCha20.h"
#endif

#ifdef mbedtls
#include <string.h>
#include <stdlib.h>
#define MBEDTLS_ALLOW_PRIVATE_ACCESS
#define MBEDTLS_CHACHA20_C
#include "mbedtls/build_info.h"
#include "mbedtls/platform.h"
#include "mbedtls/chacha20.h"
//#include "mbedtls/chacha20.c"
//#include "mbedtls/platform_util.c"
#endif

#ifdef portable8439

#define MAX_TEST_SIZE (512)

struct chacha_test_vector {
    char* title;
    uint8_t key[CHACHA20_KEY_SIZE];
    uint8_t nonce[CHACHA20_NONCE_SIZE];
    uint32_t counter;
    uint8_t plain_text[MAX_TEST_SIZE];
    uint8_t cipher_text[MAX_TEST_SIZE];
    size_t size;
};

uint8_t buffer[MAX_TEST_SIZE] = {0};

static struct chacha_test_vector e = {
    "RFC8439 2.4.2",
    {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4a,0x00,0x00,0x00,0x00},
    1,
    {
      0x4c,0x61,0x64,0x69,0x65,0x73,0x20,0x61,0x6e,0x64,0x20,0x47,0x65,0x6e,0x74,0x6c,
      0x65,0x6d,0x65,0x6e,0x20,0x6f,0x66,0x20,0x74,0x68,0x65,0x20,0x63,0x6c,0x61,0x73,
      0x73,0x20,0x6f,0x66,0x20,0x27,0x39,0x39,0x3a,0x20,0x49,0x66,0x20,0x49,0x20,0x63,
      0x6f,0x75,0x6c,0x64,0x20,0x6f,0x66,0x66,0x65,0x72,0x20,0x79,0x6f,0x75,0x20,0x6f,
      0x6e,0x6c,0x79,0x20,0x6f,0x6e,0x65,0x20,0x74,0x69,0x70,0x20,0x66,0x6f,0x72,0x20,
      0x74,0x68,0x65,0x20,0x66,0x75,0x74,0x75,0x72,0x65,0x2c,0x20,0x73,0x75,0x6e,0x73,
      0x63,0x72,0x65,0x65,0x6e,0x20,0x77,0x6f,0x75,0x6c,0x64,0x20,0x62,0x65,0x20,0x69,
      0x74,0x2e
    },
    {
      0x6e,0x2e,0x35,0x9a,0x25,0x68,0xf9,0x80,0x41,0xba,0x07,0x28,0xdd,0x0d,0x69,0x81,
      0xe9,0x7e,0x7a,0xec,0x1d,0x43,0x60,0xc2,0x0a,0x27,0xaf,0xcc,0xfd,0x9f,0xae,0x0b,
      0xf9,0x1b,0x65,0xc5,0x52,0x47,0x33,0xab,0x8f,0x59,0x3d,0xab,0xcd,0x62,0xb3,0x57,
      0x16,0x39,0xd6,0x24,0xe6,0x51,0x52,0xab,0x8f,0x53,0x0c,0x35,0x9f,0x08,0x61,0xd8,
      0x07,0xca,0x0d,0xbf,0x50,0x0d,0x6a,0x61,0x56,0xa3,0x8e,0x08,0x8a,0x22,0xb6,0x5e,
      0x52,0xbc,0x51,0x4d,0x16,0xcc,0xf8,0x06,0x81,0x8c,0xe9,0x1a,0xb7,0x79,0x37,0x36,
      0x5a,0xf9,0x0b,0xbf,0x74,0xa3,0x5b,0xe6,0xb4,0x0b,0x8e,0xed,0xf2,0x78,0x5e,0x42,
      0x87,0x4d
    },
    114
};

#endif

#ifdef chacha_avr

const uint8_t key[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};
const uint8_t nonce[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a,
    0x00, 0x00, 0x00, 0x00
};

// has to be aligned
uint8_t st[64] __attribute__((aligned(64)));
uint8_t keystream[128] = {0};

const uint8_t plaintext[114] = {0x4c, 0x61, 0x64, 0x69, 0x65, 0x73, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x47, 0x65, 0x6e, 0x74, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x20, 0x6f, 0x66, 0x20, 0x74,
    0x68, 0x65, 0x20, 0x63, 0x6c, 0x61, 0x73, 0x73, 0x20, 0x6f, 0x66, 0x20, 0x27, 0x39, 0x39, 0x3a, 0x20, 0x49, 0x66, 0x20, 0x49, 0x20, 0x63, 0x6f, 0x75,
    0x6c, 0x64, 0x20, 0x6f, 0x66, 0x66, 0x65, 0x72, 0x20, 0x79, 0x6f, 0x75, 0x20, 0x6f, 0x6e, 0x6c, 0x79, 0x20, 0x6f, 0x6e, 0x65, 0x20, 0x74, 0x69, 0x70,
    0x20, 0x66, 0x6f, 0x72, 0x20, 0x74, 0x68, 0x65, 0x20, 0x66, 0x75, 0x74, 0x75, 0x72, 0x65, 0x2c, 0x20, 0x73, 0x75, 0x6e, 0x73, 0x63, 0x72, 0x65, 0x65,
0x6e, 0x20, 0x77, 0x6f, 0x75, 0x6c, 0x64, 0x20, 0x62, 0x65, 0x20, 0x69, 0x74, 0x2e};

const uint8_t ciphertext[114] = {0x6e, 0x2e, 0x35, 0x9a, 0x25, 0x68, 0xf9, 0x80, 0x41, 0xba, 0x07, 0x28, 0xdd, 0x0d, 0x69, 0x81,
    0xe9, 0x7e, 0x7a, 0xec, 0x1d, 0x43, 0x60, 0xc2, 0x0a, 0x27, 0xaf, 0xcc, 0xfd, 0x9f, 0xae, 0x0b,
    0xf9, 0x1b, 0x65, 0xc5, 0x52, 0x47, 0x33, 0xab, 0x8f, 0x59, 0x3d, 0xab, 0xcd, 0x62, 0xb3, 0x57,
    0x16, 0x39, 0xd6, 0x24, 0xe6, 0x51, 0x52, 0xab, 0x8f, 0x53, 0x0c, 0x35, 0x9f, 0x08, 0x61, 0xd8,
    0x07, 0xca, 0x0d, 0xbf, 0x50, 0x0d, 0x6a, 0x61, 0x56, 0xa3, 0x8e, 0x08, 0x8a, 0x22, 0xb6, 0x5e,
    0x52, 0xbc, 0x51, 0x4d, 0x16, 0xcc, 0xf8, 0x06, 0x81, 0x8c, 0xe9, 0x1a, 0xb7, 0x79, 0x37, 0x36,
    0x5a, 0xf9, 0x0b, 0xbf, 0x74, 0xa3, 0x5b, 0xe6, 0xb4, 0x0b, 0x8e, 0xed, 0xf2, 0x78, 0x5e, 0x42,
0x87, 0x4d };

uint8_t buffer[114] = {0};

#endif

#ifdef ChaCha20_rfc7539
key256_t key = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

nonce96_t nonce = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a,
    0x00, 0x00, 0x00, 0x00
};

uint32_t count = 0x00000001;

uint8_t plaintext[114] = {
    0x4c, 0x61, 0x64, 0x69, 0x65, 0x73, 0x20, 0x61,
    0x6e, 0x64, 0x20, 0x47, 0x65, 0x6e, 0x74, 0x6c,
    0x65, 0x6d, 0x65, 0x6e, 0x20, 0x6f, 0x66, 0x20,
    0x74, 0x68, 0x65, 0x20, 0x63, 0x6c, 0x61, 0x73,
    0x73, 0x20, 0x6f, 0x66, 0x20, 0x27, 0x39, 0x39,
    0x3a, 0x20, 0x49, 0x66, 0x20, 0x49, 0x20, 0x63,
    0x6f, 0x75, 0x6c, 0x64, 0x20, 0x6f, 0x66, 0x66,
    0x65, 0x72, 0x20, 0x79, 0x6f, 0x75, 0x20, 0x6f,
    0x6e, 0x6c, 0x79, 0x20, 0x6f, 0x6e, 0x65, 0x20,
    0x74, 0x69, 0x70, 0x20, 0x66, 0x6f, 0x72, 0x20,
    0x74, 0x68, 0x65, 0x20, 0x66, 0x75, 0x74, 0x75,
    0x72, 0x65, 0x2c, 0x20, 0x73, 0x75, 0x6e, 0x73,
    0x63, 0x72, 0x65, 0x65, 0x6e, 0x20, 0x77, 0x6f,
    0x75, 0x6c, 0x64, 0x20, 0x62, 0x65, 0x20, 0x69,
    0x74, 0x2e
};

uint8_t ciphertext[114] = {
    0x6e, 0x2e, 0x35, 0x9a, 0x25, 0x68, 0xf9, 0x80,
    0x41, 0xba, 0x07, 0x28, 0xdd, 0x0d, 0x69, 0x81,
    0xe9, 0x7e, 0x7a, 0xec, 0x1d, 0x43, 0x60, 0xc2,
    0x0a, 0x27, 0xaf, 0xcc, 0xfd, 0x9f, 0xae, 0x0b,
    0xf9, 0x1b, 0x65, 0xc5, 0x52, 0x47, 0x33, 0xab,
    0x8f, 0x59, 0x3d, 0xab, 0xcd, 0x62, 0xb3, 0x57,
    0x16, 0x39, 0xd6, 0x24, 0xe6, 0x51, 0x52, 0xab,
    0x8f, 0x53, 0x0c, 0x35, 0x9f, 0x08, 0x61, 0xd8,
    0x07, 0xca, 0x0d, 0xbf, 0x50, 0x0d, 0x6a, 0x61,
    0x56, 0xa3, 0x8e, 0x08, 0x8a, 0x22, 0xb6, 0x5e,
    0x52, 0xbc, 0x51, 0x4d, 0x16, 0xcc, 0xf8, 0x06,
    0x81, 0x8c, 0xe9, 0x1a, 0xb7, 0x79, 0x37, 0x36,
    0x5a, 0xf9, 0x0b, 0xbf, 0x74, 0xa3, 0x5b, 0xe6,
    0xb4, 0x0b, 0x8e, 0xed, 0xf2, 0x78, 0x5e, 0x42,
    0x87, 0x4d
};

static uint8_t plain_data[114] = {0};
static uint8_t cipher_data[114] = {0};
static uint8_t buffer[114] = {0};
ChaCha20_Ctx ctx;

void chacha20_init()
{
    #ifdef ChaCha20_rfc7539
    memcpy(plain_data, plaintext, 114);
    memcpy(cipher_data, ciphertext, 114);
    memset(buffer, 0, 114);
    #endif
}
#endif


void test_enc()
{
#ifdef portable8439
    chacha20_xor_stream(buffer, e.plain_text, e.size, e.key, e.nonce, e.counter);
#endif

#ifdef chacha_avr

    chacha20_block(st, key, nonce, 1);
    memcpy(keystream, st, 64);

    chacha20_block(st, key, nonce, 2);
    memcpy(keystream + 64, st, 64);

    for(uint8_t i = 0; i < sizeof(plaintext); ++i)
    {
        buffer[i] = plaintext[i]^keystream[i];
    }


#endif

#ifdef ChaCha20_rfc7539
    ChaCha20_init(&ctx, key, nonce, count); // need to include state formation in the timing measurements
    ChaCha20_xor(&ctx, plain_data, 114);
    memcpy(buffer, plain_data, 114);
#endif

#ifdef mbedtls
    mbedtls_chacha20_crypt(key, nonce, count, size, plaintext, buffer);
#endif
}

void test_dec()
{
#ifdef portable8439
  chacha20_xor_stream(buffer, e.cipher_text, e.size, e.key, e.nonce, e.counter);
#endif

#ifdef chacha_avr
    chacha20_block(st, key, nonce, 1);
    memcpy(keystream, st, 64);

    chacha20_block(st, key, nonce, 2);
    memcpy(keystream + 64, st, 64);

    for(uint8_t i = 0; i < sizeof(ciphertext); ++i)
    {
        buffer[i] = ciphertext[i]^keystream[i];
    }
#endif

#ifdef ChaCha20_rfc7539
    ChaCha20_init(&ctx, key, nonce, count);
    ChaCha20_xor(&ctx, cipher_data, 114);
    memcpy(buffer, cipher_data, 114);
#endif

#ifdef mbedtls
    mbedtls_chacha20_crypt(key, nonce, count, size, ciphertext, buffer1);
#endif
}

///******************************
// *
// * Function to verify encryption
// *
// ******************************/
int check_encrypt() {
#ifdef portable8439
    if (memcmp(buffer, e.cipher_text, e.size) != 0) {
        return -1;
    }
    else {
        return 1;
    }
    #endif

    #ifdef chacha_avr
    if(memcmp(ciphertext, buffer, sizeof(ciphertext)) != 0) {
        return -1;
    }
    else {
        return 1;
    }
    #endif

    #ifdef ChaCha20_rfc7539
    if(memcmp(ciphertext, buffer, sizeof(ciphertext)) != 0) {
        return -1;
    }
    else {
        return 1;
    }
    #endif

    #ifdef mbedtls
    if (memcmp(buffer, ciphertext, size) != 0) {
       return -1;
    }
    else {
      return 1;
    }
    #endif
    return 0;
}
//
///******************************
// *
// * Function to verify decryption
// *
// ******************************/
int check_decrypt() {
#ifdef portable8439
    if (memcmp(buffer, e.plain_text, e.size) != 0) {
        //            printf("failed\n");
        return -1;
    }
    else {
        //            printf("success\n");
        return 1;
    }
    #endif

    #ifdef chacha_avr
    if(memcmp(plaintext, buffer, sizeof(plaintext)) != 0) {
        return -1;
    }
    else {
        return 1;
    }
    #endif

    #ifdef ChaCha20_rfc7539
    if(memcmp(plaintext, buffer, sizeof(plaintext)) != 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
    #endif

    #ifdef mbedtls
    if (memcmp(buffer1, plaintext, size) != 0) {
       return -1;
    }
    else {
      return 1;
    }
    #endif
    return 0;
}

#ifdef riscv
double cycles_to_ms(unsigned long long cycles) {
    double frequency = 32000000.0; // HiFive1 Rev B board has a 32MHz clock
    return (cycles / frequency) * 1000.0;
}
#endif

#ifdef saml11
void start_timer_0_16_bit(void)
{
    GCLK->PCHCTRL[14].reg = (GCLK_PCHCTRL_CHEN|GCLK_PCHCTRL_GEN_GCLK0);
    TC0->COUNT16.CTRLA.bit.ENABLE = 0; // CNTLA is write protected to so we have to disable it before writing.
    while(TC0->COUNT16.SYNCBUSY.reg & TC_SYNCBUSY_ENABLE);
    TC0->COUNT16.CTRLA.bit.MODE = 0x1; // set the counter to be in 32 bit mode.
    TC0->COUNT16.DBGCTRL.bit.DBGRUN = 0x1; // keep timer running even when the device is halted.
    TC0->COUNT16.CTRLA.bit.ENABLE = 0x1; // start the counter
    while(TC0->COUNT16.SYNCBUSY.bit.ENABLE);

}

uint16_t read_time_0_16_bit(void)
{
    TC0->COUNT16.CTRLBSET.bit.CMD = 0X4;// READSYNC
    while(TC0->COUNT16.SYNCBUSY.reg & TC_SYNCBUSY_CTRLB);
    return TC0->COUNT16.COUNT.reg;

}

void start_timer_0_32_bit(void)
{
    /************************************************************************/
    /* 16 bit is not enough for most cases
    So this function makes it a 32 bit time.
    TC0 and TC1; TC1 is just a slave device.                                     */
    /************************************************************************/
    GCLK->PCHCTRL[14].reg = (GCLK_PCHCTRL_CHEN|GCLK_PCHCTRL_GEN_GCLK0);
    TC0->COUNT32.CTRLA.bit.ENABLE = 0; // CNTLA is write protected to so we have to disable it before writing.
    while(TC0->COUNT32.SYNCBUSY.reg & TC_SYNCBUSY_ENABLE);

    TC0->COUNT32.CTRLA.bit.MODE = 0x2; // set the counter to be in 32 bit mode.
    TC0->COUNT32.DBGCTRL.bit.DBGRUN = 0x1; // keep timer running even when the device is halted.
    TC0->COUNT32.CTRLA.bit.ENABLE = 0x1; // start the counter
    while(TC0->COUNT32.SYNCBUSY.bit.ENABLE);// wait for it to be activated
}

volatile uint32_t read_time_0_32_bit(void)
{ /**Dumps the timer cycles to a variable*/
    TC0->COUNT32.CTRLBSET.bit.CMD = 0X4;// READSYNC
    while(TC0->COUNT32.SYNCBUSY.reg & TC_SYNCBUSY_CTRLB);
    return TC0->COUNT32.COUNT.reg; // return count register.
}
#endif

#ifdef apollo3
// -- UART Stuff --
void *phUART;

#define CHECK_ERRORS(x)                                                       \
    if ((x) != AM_HAL_STATUS_SUCCESS)                                         \
    {                                                                         \
        error_handler(x);                                                     \
    }

volatile uint32_t ui32LastError;

// Catch HAL errors
void error_handler(uint32_t ui32ErrorStatus)
{
    ui32LastError = ui32ErrorStatus;
    while (1);
}

// UART buffers
uint8_t g_pui8TxBuffer[256];
uint8_t g_pui8RxBuffer[2];

// UART configuration
const am_hal_uart_config_t g_sUartConfig =
{
    // Standard UART settings: 115200-8-N-1
    .ui32BaudRate = 115200,
    .ui32DataBits = AM_HAL_UART_DATA_BITS_8,
    .ui32Parity = AM_HAL_UART_PARITY_NONE,
    .ui32StopBits = AM_HAL_UART_ONE_STOP_BIT,
    .ui32FlowControl = AM_HAL_UART_FLOW_CTRL_NONE,

    // Set TX and RX FIFOs to interrupt at half-full.
    .ui32FifoLevels = (AM_HAL_UART_TX_FIFO_1_2 |
                       AM_HAL_UART_RX_FIFO_1_2),

    // Buffers
    .pui8TxBuffer = g_pui8TxBuffer,
    .ui32TxBufferSize = sizeof(g_pui8TxBuffer),
    .pui8RxBuffer = g_pui8RxBuffer,
    .ui32RxBufferSize = sizeof(g_pui8RxBuffer),
};

// UART0 interrupt handler
void am_uart_isr(void)
{
    // Service the FIFOs as necessary, and clear the interrupts
    uint32_t ui32Status, ui32Idle;
    am_hal_uart_interrupt_status_get(phUART, &ui32Status, true);
    am_hal_uart_interrupt_clear(phUART, ui32Status);
    am_hal_uart_interrupt_service(phUART, ui32Status, &ui32Idle);
}

// UART print string
void uart_print(char *pcStr)
{
    uint32_t ui32StrLen = 0;
    uint32_t ui32BytesWritten = 0;

    // Measure the length of the string
    while (pcStr[ui32StrLen] != 0){
        ui32StrLen++;
    }

    // Print the string via the UART
    const am_hal_uart_transfer_t sUartWrite =
    {
        .ui32Direction = AM_HAL_UART_WRITE,
        .pui8Data = (uint8_t *) pcStr,
        .ui32NumBytes = ui32StrLen,
        .ui32TimeoutMs = 0,
        .pui32BytesTransferred = &ui32BytesWritten,
    };

    CHECK_ERRORS(am_hal_uart_transfer(phUART, &sUartWrite));

    if (ui32BytesWritten != ui32StrLen)
    {
        // Couldn't send the whole string!!
        while(1);
    }
}
 // -- End UART Stuff --

void init(void){
//    am_util_id_t sIdDevice;
//    uint32_t ui32StrBuf;

    // Set the clock frequency.
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);

    // Set the default cache configuration
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();

    // Configure the board for low power operation.
    // am_bsp_low_power_init();

    // Initialize the printf interface for UART output
    CHECK_ERRORS(am_hal_uart_initialize(0, &phUART));
    CHECK_ERRORS(am_hal_uart_power_control(phUART, AM_HAL_SYSCTRL_WAKE, false));
    CHECK_ERRORS(am_hal_uart_configure(phUART, &g_sUartConfig));

    // Enable the UART pins
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_TX, g_AM_BSP_GPIO_COM_UART_TX);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_RX, g_AM_BSP_GPIO_COM_UART_RX);

    // Enable interrupts
    NVIC_EnableIRQ((IRQn_Type)(UART0_IRQn + AM_BSP_UART_PRINT_INST));
    am_hal_interrupt_master_enable();

    // Set the main print interface to use the UART print function we defined
    am_util_stdio_printf_init(uart_print);

    //// Configure GPIO for measurement
    //am_hal_gpio_pinconfig(1, g_AM_HAL_GPIO_OUTPUT);
    //am_hal_gpio_state_write(1, AM_HAL_GPIO_OUTPUT_CLEAR);

    // Configure the system timer
    am_hal_stimer_config(AM_HAL_STIMER_CFG_THAW | AM_HAL_STIMER_HFRC_3MHZ);

}

#endif

#ifdef msp432p401r
    #ifdef UART
      /*  Function to send a character through UART*/
      void UART0_putchar(char c) {
          MAP_UART_transmitData(EUSCI_A0_BASE, c);              /* send a char */

          while(UART_queryStatusFlags(EUSCI_A0_BASE, EUSCI_A_UART_BUSY));  /* wait for transmit buffer empty */
      }

      /*  Function to send a string through UART by sending each character in the string. */
      void UART0_puts(char* s) {
          while (*s != 0)             /* if not end of string */
          UART0_putchar(*s++);        /* send the character through UART0 */
      }

      void uart_init(void)
      {
          const eUSCI_UART_Config uartConfig =
          {
            EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
            78,                                     // BRDIV = 78
            2,                                       // UCxBRF = 2
            0,                                       // UCxBRS = 0
            EUSCI_A_UART_NO_PARITY,                  // No Parity
            EUSCI_A_UART_LSB_FIRST,                  // LSB First
            EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
            EUSCI_A_UART_MODE,                       // UART mode
            EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
          };

          /* Selecting P1.2 and P1.3 in UART mode */
          MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                  GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

          /* Setting DCO to 12MHz */
          CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

          /* Configuring UART Module */
          MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

          /* Enable UART module */
          MAP_UART_enableModule(EUSCI_A0_BASE);
      }
    #endif
#endif

#ifdef msp430fr5994
    #ifdef UART
        /*  Function to send a character through UART*/
        void UART0_putchar(char c) {

          //Load data onto buffer
          EUSCI_A_UART_transmitData(EUSCI_A0_BASE, c);
          //Wait for transmit buffer to be empty
          while(EUSCI_A_UART_queryStatusFlags(EUSCI_A0_BASE, EUSCI_A_UART_BUSY)){};
        }

        /*  Function to send a string through UART by sending each character in the string. */
        void UART0_puts(char* s) {
          while (*s != 0)             /* if not end of string */
          UART0_putchar(*s++);        /* send the character through UART0 */
        }

        void uart_init(void)
        {
            // LFXT Setup - LFXT oscillator supports ultra-low-current consumption using a 32768-Hz watch crystal.
            //Set PJ.4 and PJ.5 as Primary Module Function Input.
            /*

            * Select Port J
            * Set Pin 4, 5 to input Primary Module Function, LFXT. A watch crystal connects to LFXIN & LFXOUT pins when enabled for LFXT configuration.
            */
            GPIO_setAsPeripheralModuleFunctionInputPin(
                GPIO_PORT_PJ,
                GPIO_PIN4 + GPIO_PIN5,
                GPIO_PRIMARY_MODULE_FUNCTION
            );

            //ACLK = BRCLK = 32.768kHz, MCLK = SMCLK = DCO = ~1MHz
            //Code works by configuring just the ACLK and LFXT as well.
            //Set DCO frequency to 1 MHz
            CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_0);
            //Set external clock frequency to 32.768 KHz
            CS_setExternalClockSource(32768,0);
            //Set ACLK=LFXT
            CS_initClockSignal(CS_ACLK,CS_LFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);
            //Set SMCLK = DCO with frequency divider of 1
            CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
            //Set MCLK = DCO with frequency divider of 1
            CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
            //Start XT1 with no time out
            CS_turnOnLFXT(CS_LFXT_DRIVE_0);

            //Configure UART pins
            //Set P2.0 and P2.1 as Secondary Module Function Input for MSP430FR5994.
            /*

            * Select Port 2d
            * Set Pin 0, 1 to input Secondary Module Function, (UCA0TXD/UCA0SIMO, UCA0RXD/UCA0SOMI).
            */
            GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P2,
            GPIO_PIN0 + GPIO_PIN1,
            GPIO_SECONDARY_MODULE_FUNCTION
            );

            // Configure UART 9600 baud rate with ACLK = LFXT = 32.768kHz
            EUSCI_A_UART_initParam param = {0};
            param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_ACLK;
            param.clockPrescalar = 3;
            param.firstModReg = 0;
            param.secondModReg = 92;
            param.parity = EUSCI_A_UART_NO_PARITY;
            param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
            param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
            param.uartMode = EUSCI_A_UART_MODE;
            param.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

            if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A0_BASE, &param)) {
                return;
            }

            EUSCI_A_UART_enable(EUSCI_A0_BASE);

        }
    #endif
#endif

#ifdef msp430f5529
#ifdef UART
    /*  Function to send a character through UART*/
    void UART1_putchar(char c) {

        //Load data onto buffer
        USCI_A_UART_transmitData(USCI_A1_BASE, c);
        //Wait for transmit buffer to be empty
        while(USCI_A_UART_queryStatusFlags(USCI_A1_BASE, USCI_A_UART_BUSY)){};

    }

    /*  Function to send a string through UART by sending each character in the string. */
    void UART1_puts(char* s) {
        while (*s != 0)             /* if not end of string */
        UART1_putchar(*s++);        /* send the character through UART0 */
    }

    void uart_init(void)
    {
         //P4.4,5 = USCI_A1 TXD/RXD for MSP430F5529
        GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P4,
            GPIO_PIN4 + GPIO_PIN5
            );

        //Baudrate = 9600, clock freq = 1.048MHz
        //UCBRx = 109, UCBRFx = 0, UCBRSx = 2, UCOS16 = 0
        // N = clock freq/Baudrate
        USCI_A_UART_initParam param = {0};
        param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
        param.clockPrescalar = 109;
        param.firstModReg = 0;
        param.secondModReg = 2;
        param.parity = USCI_A_UART_NO_PARITY;
        param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
        param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
        param.uartMode = USCI_A_UART_MODE;
        param.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

        //Check UART initialization
        if (STATUS_FAIL == USCI_A_UART_init(USCI_A1_BASE, &param)){
                return;
            }

        //Enable UART module for operation
        USCI_A_UART_enable(USCI_A1_BASE);

    }
    #endif
#endif

#ifdef adafruitm0express
    void setup() {
      Serial.begin(9600);
    }
#endif

//Use void loop instead of int main for arduino
//void loop()
 int main (void)
{
#ifdef ChaCha20_rfc7539
    chacha20_init();
#endif

#ifdef mbedtls
    mbedtls_chacha20_init(&ctx);
#endif

#ifdef riscv
    int hartid = metal_cpu_get_current_hartid();
    unsigned long long start_cycle_count, end_cycle_count;

    // Get the start cycle count
    if (metal_timer_get_cyclecount(hartid, &start_cycle_count) != 0) {
        printf("Failed to get the start cycle count.\n");
        return 1;
    }
#endif

#ifdef saml11
    atmel_start_init();
    #ifdef TIMER
        volatile uint32_t a = 0;
        start_timer_0_32_bit();
    #endif
#endif

#ifdef apollo3
    init();
    am_hal_gpio_state_write(1, AM_HAL_GPIO_OUTPUT_CLEAR);
    am_util_delay_us(20);
    uint32_t startClock, stopClock, shaTime;

    //------------
    startClock = am_hal_stimer_counter_get();
    am_hal_gpio_state_write(1, AM_HAL_GPIO_OUTPUT_SET);
    //------------
#endif

#if defined msp432p401r || defined msp430fr5994 || defined msp430f5529
    board_init();
#ifdef UART
    uart_init();
#endif
    startTimer();
#endif

#ifdef adafruitm0express
  /** Measure the starting time **/
  setup();
  unsigned long start, finished, elapsed;
  start = micros();
#endif

  /** test CHACHA **/
  // print key
    printf("Key: ");
    for (int i = 0; i < sizeof(key); i++) {
        printf("%02x", key[i]);
    }
    printf("\r\n");
    // print nonce
    printf("Nonce: ");
    for (int i = 0; i < sizeof(nonce); i++) {
        printf("%02x", nonce[i]);
    }
    printf("\r\n");
  // print plaintext
    printf("Plaintext: ");
    for (int i = 0; i < sizeof(plaintext); i++) {
        printf("%02x", plaintext[i]);
    }
    printf("\r\n");
    test_enc();
    // print ciphertext
    printf("Ciphertext: ");
    for (int i = 0; i < sizeof(ciphertext); i++) {
        printf("%02x", buffer[i]);
    }
    printf("\r\n");

    int verify = check_encrypt();
    printf("Encryption verified: %s\r\n", verify == 1 ? "yes" : "no");
    
    
    test_dec();
    // print plaintext
    printf("Plaintext: ");
    for (int i = 0; i < sizeof(plaintext); i++) {
        printf("%02x", buffer[i]);
    }
    printf("\r\n");
    verify = check_decrypt();
    printf("Decryption: %s\r\n", verify == 1 ? "yes" : "no");

#ifdef riscv
    // Get the end cycle count
    if (metal_timer_get_cyclecount(hartid, &end_cycle_count) != 0) {
        printf("Failed to get the end cycle count.\n");
        return 1;
    }

    // Check for timer overflow
    unsigned long long elapsed_cycles;

    if (end_cycle_count >= start_cycle_count) {
        elapsed_cycles = end_cycle_count - start_cycle_count;
    } else {
        elapsed_cycles = (0xFFFFFFFFFFFFFFFF - start_cycle_count)
                + end_cycle_count + 1;
    }

    // Convert clock cycle count to milliseconds
    double elapsed_ms = cycles_to_ms(elapsed_cycles);

    //Time is printed in us to avoid floating point computations
    printf("Function execution time: %d us\n", (unsigned int)(elapsed_ms*1000));
//       volatile unsigned int res = check_encrypt();
//       volatile unsigned int res = check_decrypt();

#endif

#ifdef saml11
    #ifdef TIMER
        a = read_time_0_32_bit(); // add a watch point to this variable if you want to see it live.
        *(uint32_t *)(0x2000383C) = a; // store the timer data in the SRAM at @ 0x2000383C; plan is to get it out of the
        printf("%s", welcoming_str);
        printf("%lu\n",a);
    #endif
#endif

#ifdef apollo3
    //-----------
    am_hal_gpio_state_write(1, AM_HAL_GPIO_OUTPUT_CLEAR);
    stopClock = am_hal_stimer_counter_get();

    am_hal_gpio_state_write(46, AM_HAL_GPIO_OUTPUT_SET);

    if(stopClock < startClock){ // Overflow
        shaTime = (0xFFFFFFFF - startClock) + stopClock;
    }else{
        shaTime = stopClock - startClock;
    }

    shaTime /= 3;   // Convert from 3 MHz frequency to us

    am_util_stdio_terminal_clear();
    am_util_stdio_printf("Time: %u us\r\n", shaTime);

        //-----------
#endif

#if defined msp432p401r || defined msp430fr5994
    volatile unsigned int elapsed = getElapsedTime();

    #ifdef UART
        char buf[20];
        sprintf(buf, "Value: %d\n", elapsed); //Divide by 10 to get ms
        UART0_puts(buf);
    #endif
#endif

#if defined msp430f5529
    volatile unsigned int elapsed = getElapsedTime();

    #ifdef UART
        char buf[20];
        sprintf(buf, "Value: %d\n", elapsed); //Divide by 10 to get ms
        UART1_puts(buf);
    #endif
#endif

#ifdef adafruitm0express
  /** Calculate the elapsed time **/
  finished = micros();
  elapsed = finished - start;
  Serial.print("Time taken by the task: ");
  Serial.println(elapsed);

  // wait a second so as not to send massive amounts of data
  delay(1000);
#endif

      return 0;
}
