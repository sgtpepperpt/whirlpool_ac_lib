#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>

unsigned char chars_to_byte (char a, char b);
void remove_char(char *str, char to_remove);
void byte_to_bin_str(char* bin_str, unsigned char c);
unsigned char xor(unsigned char* buf, size_t len);
unsigned char reverse(unsigned char b);

#endif
