#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "lw.h"
#include "inputs.h"

#define ITERATIONS 50

void print_hex_array(const char *preamble, const uint8_t *array, const int len)
{
    if (preamble != NULL)
    {
        printf("%s", preamble);
    }

    for (int i = 0; i < len; i++)
    {
        printf("%02X ", array[i]);
        if ((i + 1) % 32 == 0)
        {
            printf("\r\n");
        }
    }
    printf("\r\n");
}

benchmark_hash_t benchmark_main(void)
{
    benchmark_hash_t benchmark_hash_ret = 0;

#if HASH_TEST
    hash_result_t benchmark_hash;
    hash_ctx_t benchmark_hash_ctx;
    hash_init(&benchmark_hash_ctx);
#endif  // HASH_TEST

    lw_frame_t frame;
    lw_key_grp_t kgrp;
    int status;

    lw_init(US915);

    // Print input data parameters
    print_hex_array("App EUI: ", g_app_eui, 8);
    print_hex_array("Device EUI: ", g_dev_eui, 8);
    print_hex_array("App Key: ", g_app_key, 16);
    printf("\r\n");

    memset((uint8_t *)&frame, 0, sizeof(lw_frame_t));
    lw_cpy(frame.deveui, g_dev_eui, 8);
    lw_cpy(frame.appeui, g_app_eui, 8);
    frame.pl.jr.devnonce.data = g_devnonce;

    // Join Accept message
    printf("Parsing the following Join Accept message %u times:\r\n", ITERATIONS);
    print_hex_array(NULL, g_input_ja_msg, g_input_ja_msg_len);
    printf("\r\n");

    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        status = lw_parse(&frame, g_input_ja_msg, g_input_ja_msg_len);
#if HASH_TEST
        hash_update(&benchmark_hash_ctx, &frame.pl, sizeof(frame.pl));
#endif
    }
    if (status == LW_OK)
    {
        printf("App nonce: 0x%06X\r\n", frame.pl.ja.appnonce.data);
        printf("Net ID: 0x%06X\r\n", frame.pl.ja.netid.data);
        printf("Dev Address: 0x%08X\r\n", frame.pl.ja.devaddr.data);
        printf("RX2DataRate: %u\r\n", frame.pl.ja.dlsettings.bits.rx2dr);
        printf("RX1DRoffset: %u\r\n", frame.pl.ja.dlsettings.bits.rx1droft);
        print_hex_array("App Session Key: ", frame.pl.ja.appskey, 16);
        print_hex_array("Network Session Key: ", frame.pl.ja.nwkskey, 16);
    }
    else
    {
        printf("JOIN ACCEPT PARSE error (%d)", status);
    }
    printf("\r\n");

    kgrp.nwkskey = g_nwk_skey;
    kgrp.flag.bits.nwkskey = 1;
    kgrp.appskey = g_app_skey;
    kgrp.flag.bits.appskey = 1;
    kgrp.appkey = g_app_key;
    kgrp.flag.bits.appkey = 1;
    lw_set_key(&kgrp);

    // Data downlink message
    printf("Parsing the following unconfirmed data down message %u times:\r\n", ITERATIONS);
    print_hex_array(NULL, g_input_ud_msg, g_input_ud_msg_len);
    printf("\r\n");

    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        status = lw_parse(&frame, g_input_ud_msg, g_input_ud_msg_len);
#if HASH_TEST
        hash_update(&benchmark_hash_ctx, &frame.pl, sizeof(frame.pl));
#endif
    }
    if (status == LW_OK)
    {
        printf("Dev Address: 0x%08X\r\n", frame.pl.mac.devaddr.data);
        printf("FCnt: %u\r\n", frame.pl.mac.fcnt);
        printf("Port: %u\r\n", frame.pl.mac.fport);
        printf("ACK: %u\r\n", frame.pl.mac.fctrl.ul.ack);
        printf("ADR: %u\r\n", frame.pl.mac.fctrl.ul.adr);
        printf("ADRACKReq: %u\r\n", frame.pl.mac.fctrl.ul.adrackreq);
        printf("Data length: %u\r\n", frame.pl.mac.flen);
        print_hex_array("Data (hex): \r\n", frame.pl.mac.fpl, frame.pl.mac.flen);
        printf("Data (text):\r\n%s\r\n", frame.pl.mac.fpl);
        printf("\r\n");
    }
    else
    {
        printf("DATA MESSAGE PARSE error (%d)", status);
    }

#if HASH_TEST
    hash_final(benchmark_hash, &benchmark_hash_ctx);
    benchmark_hash_ret = hash_get_lowest32bits(benchmark_hash);
#endif  // HASH_TEST

    return benchmark_hash_ret;
}
