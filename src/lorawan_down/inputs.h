#ifndef INPUTS_H
#define INPUTS_H

/* Parameters to generate Join Accept message
 * --anonce 0xBB1872
 * --devaddr 0x12345678
 * --netid 0x02468A
 * --appkey 0x2B7E151628AED2A6ABF7158809CF4F3C
 * --dnonce 0xDC25
*/

extern uint8_t g_dev_eui[];
extern uint8_t g_app_eui[];
extern uint8_t g_app_key[];
extern uint8_t g_nwk_skey[];
extern uint8_t g_app_skey[];
extern const uint32_t g_port;
extern const uint32_t g_fcnt;
extern const uint16_t g_devnonce;
extern uint8_t g_input_ja_msg[];
extern uint32_t g_input_ja_msg_len;
extern uint8_t g_input_ud_msg[];
extern uint32_t g_input_ud_msg_len;

#endif // INPUTS_H
