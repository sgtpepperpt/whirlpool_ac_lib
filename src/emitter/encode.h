#ifndef __ENCODE_H__
#define __ENCODE_H__

#include "ir_command.h"

void ir_send(int gpio_pin, ir_command* ir);

#endif
