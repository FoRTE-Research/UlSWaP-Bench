#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "common.h"

extern void hexstring(uint32_t num);

#define PRECISION 1000      // 3 decimal places
static uint32_t g_checksum = 0;
static const char* g_format_modifiers = "1234567890.+#z<>l-h";
static const char* g_unsupported_modifiers = "Ljt";

uint32_t get_benchmark_checksum(void)
{
    return g_checksum;
}

static inline void add_to_checksum(uint32_t value)
{
    g_checksum += value;
}

static inline bool is_modifier(char c)
{
    return strchr(g_format_modifiers, c) != NULL;
}

static inline bool is_unsupported_modifier(char c)
{
    return strchr(g_unsupported_modifiers, c) != NULL;
}

static double reduce_double_precision(double value)
{
    return round(value * PRECISION) / PRECISION;
}

void printf_checksum(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char c = *fmt;
    char *fmt_start = (char *)fmt;
    while (c != '\0')
    {
        if (c == '%')
        {
            c = *(++fmt);

            // Skip format modifiers
            while (is_modifier(c))
            {
                c = *(++fmt);
            }
            if (is_unsupported_modifier(c))
            {
                puts("Unsupported printf modifier encountered for checksum testing:");
                putchar(c);
                puts("\r\n");
                va_end(args);
                return;
            }
            if (c == 's')
            {
                const char *str = va_arg(args, const char *);
                if (str != NULL)
                {
                    while (*str != '\0')
                    {
                        add_to_checksum((uint8_t)(*str));
                        str++;
                    }
                }
            }
            else if (c == 'f')
            {
                double num = va_arg(args, double);
                num = reduce_double_precision(num);
                uint32_t upper32 = *((uint32_t *)&num);
                uint32_t lower32 = *((uint32_t *)&num + 1);
                add_to_checksum(upper32);
                add_to_checksum(lower32);
            }
            else if ((c == 'd') || (c == 'i') || (c == 'u') || (c == 'x') || (c == 'X') || (c == 'c'))
            {
                if (*(fmt - 1) == 'l')
                {
                    if ((fmt - 2 > fmt_start) && (*(fmt - 2) == 'l'))
                    {
                        long long num = va_arg(args, long long);
                        add_to_checksum((uint32_t)(num >> 32));
                        add_to_checksum((uint32_t)(num & 0xFFFFFFFF));
                    }
                    else
                    {
                        puts("%l not supported for architecture independent checksum testing. Use %ll instead.");
                    }
                }
                else
                {
                    int num = va_arg(args, int);
                    add_to_checksum((uint32_t)num);
                }
            }
            else if (c != '%')
            {
                puts("Unsupported printf format specifier encountered for checksum testing:");
                putchar(c);
                puts("\r\n");
                va_end(args);
                return;
            }
        }
        c = *(++fmt);
    }

    va_end(args);
}

void test_checksum(void)
{
    char     c0  = 'a';                     // 0x61
    uint32_t u0  = 123456789;               // 0x075BCD15
    int32_t  s0  = -987654321;              // 0xC521974F
    uint16_t h0  = 7890;                    // 0x1ED2
    int16_t  sh0 = -12345;                  // 0xFFFFCFC7
    uint64_t l0  = 1234567890123456789ULL;  // 0x112210F4_7DE98115
    int64_t  sl0 = -1234567890123456789LL;  // 0xEEDDEF0B_82167EEB
    double   d0  = 3.141592653589793;       // 3.142 = 0x400922D0_E5604189
    float    f0  = 2.718281828459045f;      // 2.718 = 0x4005BE76_C8B43958
    char*    str = "Hello";                 // 0x48_65_C6_C6F

    printf("Checksum test:\r\n");
    printf("%c %%, %u, %08x, %d, %hu, %llu, %hd, %lld, %7.3f, %.6f, %s\r\n",
            c0,    u0,  u0,  s0,  h0,   l0, sh0,  sl0,   d0,  f0,  str);

    uint32_t checksum = get_benchmark_checksum();
    hexstring(checksum);
    uint32_t expected_checksum = 0x61 + 0x075BCD15 + 0x075BCD15 + 0xC521974F + 0x1ED2 + 0x112210F4 + 0x7DE98115 + 0xFFFFCFC7 +
                                 0xEEDDEF0B + 0x82167EEB + 0x400922D0 + 0xE5604189 + 0x4005BE76 + 0xC8B43958 +
                                 'H' + 'e' + 'l' + 'l' + 'o';

    if (checksum == expected_checksum)
    {
        puts("Checksum test passed.");
    }
    else
    {
        puts("Checksum test failed.");
    }
}
