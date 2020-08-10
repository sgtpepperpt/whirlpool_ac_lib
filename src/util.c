#include "util.h"

unsigned char char_to_byte(char c) {
    switch (c) {
        case '0': return 0x00;
        case '1': return 0x01;
        case '2': return 0x02;
        case '3': return 0x03;
        case '4': return 0x04;
        case '5': return 0x05;
        case '6': return 0x06;
        case '7': return 0x07;
        case '8': return 0x08;
        case '9': return 0x09;
        case 'A': return 0x0A;
        case 'B': return 0x0B;
        case 'C': return 0x0C;
        case 'D': return 0x0D;
        case 'E': return 0x0E;
        case 'F': return 0x0F;
        default: return 0x00;
    }
}

unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

unsigned char chars_to_byte (char a, char b) {
    return char_to_byte(a) << 4 | char_to_byte(b);
}

void remove_char(char *str, char to_remove) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != to_remove)
            dst++;
    }
    *dst = 0x00;
}

void byte_to_bin_str(char* bin_str, unsigned char c) {
    for (int i = 7; i >= 4; --i)
        bin_str[7 - i] = (c & (1 << i))? '1' : '0';

    bin_str[4] = ' ';

    for (int i = 3; i >= 0; --i)
        bin_str[7 - i + 1] = (c & (1 << i))? '1' : '0';
    bin_str[9] = 0x00;
}

unsigned char xor(unsigned char* buf, size_t len) {
    unsigned char b = 0x00;
    for (size_t i = 0; i < len; i++)
        b ^= buf[i];

    return b;
}
