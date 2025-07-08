#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "lw.h"
#include "inputs.h"
#define ITERATIONS 50

void print_hex_array(const char* preamble, const uint8_t *array, const int len)
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

int32_t create_message(lw_frame_t *frame, uint8_t *msg, int32_t *len, uint32_t iterations)
{
    int32_t ret = 0;
    for (uint32_t i = 0; i < iterations; i++)
    {
        ret = lw_pack(frame, msg, len);
    }
    return ret;
}

void print_output_message_details(const lw_frame_t *frame, const uint8_t *msg, const int32_t len)
{
    print_hex_array("LoRaWAN Message:\r\n", msg, len);
    printf("LoRaWAN message length: %d\r\n", len);
    print_hex_array("MIC: ", frame->mic.buf, 4);
}

int benchmark_main()
{
    lw_node_t endnode;
    lw_frame_t frame;
    uint8_t msg[256];
    int32_t len;

    lw_init(US915);

    // Print input data parameters
    printf("Device Address: %#08X\r\n", g_dev_address);
    print_hex_array("App EUI: ", g_app_eui, 8);
    print_hex_array("Device EUI: ", g_dev_eui, 8);
    print_hex_array("App Key: ", g_app_key, 16);
    print_hex_array("App Session Key: ", g_app_skey, 16);
    print_hex_array("Network Session Key: ", g_nwk_skey, 16);
    printf("Port: %u\r\n", g_port);
    printf("Frame Counter: %u\r\n", g_fcnt);
    printf("\r\n");

    // End node
    memset((uint8_t*)&endnode, 0, sizeof(lw_node_t));
    lw_cpy(endnode.appeui,  g_app_eui, 8);
    lw_cpy(endnode.deveui,  g_dev_eui, 8);
    memcpy(endnode.appkey,  g_app_key, 16);
    memcpy(endnode.nwkskey, g_nwk_skey, 16);
    memcpy(endnode.appskey, g_app_skey, 16);
    endnode.devnonce.data = g_devnonce;

    // Frame
    memset((uint8_t *)&frame, 0, sizeof(lw_frame_t));
    lw_cpy(frame.deveui, g_dev_eui, 8);
    lw_cpy(frame.appeui, g_app_eui, 8);
    frame.node = &endnode;

    // Specific to join request
    frame.pl.jr.devnonce.data = g_devnonce;

    // Specific to unconfirmed data up
    frame.pl.mac.devaddr.data       = g_dev_address;
    frame.pl.mac.fctrl.ul.ack       = 0;
    frame.pl.mac.fctrl.ul.adr       = 0;
    frame.pl.mac.fctrl.ul.adrackreq = 0;
    frame.pl.mac.fctrl.ul.classb    = 0;
    frame.pl.mac.fcnt               = g_fcnt;
    frame.pl.mac.fport              = g_port;
    frame.pl.mac.flen               = g_input_data_len;
    memcpy(frame.pl.mac.fpl, g_input_data, g_input_data_len);

    frame.mhdr.bits.mtype = LW_MTYPE_JOIN_REQUEST;
    printf("Creating join request message %u times with DevNonce value 0x%08X\r\n\r\n", ITERATIONS, g_devnonce);
    if (create_message(&frame, msg, &len, ITERATIONS) < 0)
    {
        printf("Error creating join request message\r\n");
    }
    else
    {
        print_output_message_details(&frame, msg, len);
    }
    printf("\r\n");

    frame.mhdr.bits.mtype = LW_MTYPE_MSG_UP;
    printf("Creating unconfirmed data up message %u times with the following %u byte input data:\r\n",
           ITERATIONS,
           g_input_data_len);
    print_hex_array(NULL, g_input_data, g_input_data_len);
    printf("\r\n");

    if (create_message(&frame, msg, &len, ITERATIONS) < 0)
    {
        printf("Error creating unconfirmed data up message\r\n");
    }
    else
    {
        print_output_message_details(&frame, msg, len);
    }
    printf("\r\n");
    return 0;
}
