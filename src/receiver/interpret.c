#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpret.h"
#include "util.h"

int byte_decode_hour(unsigned char a) {
    const int timer_hour_vals[24] = { 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22 };

    for (size_t i = 0; i < 24; i++) {
        if (a == timer_hour_vals[i])
            return i + 1;
    }

    return -1;
}

int interpret_2(ir_command* command, unsigned char b) {
    // bit 0 is toggle swing
    command->is_toggle_swing = (b & 0x80) >> 7;

    // bit 1-3 is sixth sense temp
    unsigned char sixth_sense_temp = (b & 0x70) >> 4;
    switch (sixth_sense_temp) {
        case 0x02:
        case 0x01:
        case 0x00:
        case 0x05:
        case 0x06:
            command->sixth_sense_temperature = sixth_sense_temp;
            break;
        default:
            printf("byte 2 part 6th sense wrong %02X\n", sixth_sense_temp);
            return 1;
    }

    // bit 4 is sleep mode on
    command->is_sleep_on = (b & 0x08) >> 3;

    // bit 5 is toggle on/off
    command->is_toggle_on_off = (b & 0x04) >> 2;

    // bits 6-7 is fan speed
    unsigned char fan = (b & 0x03);
    switch (fan) {
        case 0x00:
        case 0x02:
        case 0x01:
        case 0x03:
            command->fan_speed = fan;
            break;
        default:
            printf("byte 2 part fan wrong %02X\n", fan);
            return 1;
    }

    return 0;
}

int interpret_3(ir_command* command, unsigned char b) {
    char* s;

    // part 1 is temp
    unsigned char temperature_bin = (b & 0xF0) >> 4;
    if (temperature_bin > 0x0E) {
        printf("byte 3 part 1 wrong %02X\n", temperature_bin);
        return 1;
    }

    command->temperature = (int)temperature_bin;

    // part 2, bit 1 means is timer on
    command->is_timer_on = (b & 0x08) >> 3;

    // part 2, bits 2-4 is mode
    unsigned char mode = (b & 0x07);
    switch (mode) {
        case 0x01:
        case 0x00:
        case 0x02:
        case 0x04:
            command->mode = mode;
            break;
        default:
            printf("byte 3 part 2 wrong %02X\n", mode);
            return 1;
    }

    return 0;
}

int interpret_4(ir_command* command, unsigned char b) {
    // first bit is turn on 6th sense
    unsigned char toggle_6th = b & 0x80;
    if (toggle_6th != 0x80 && toggle_6th != 0x00) {
        printf("byte 4 first bit wrong %02X\n", toggle_6th);
        return 1;
    }

    command->is_turn_sixth_sense_on = (b == 0x80);

    // timer set hour (0x7F to get last 7 bits)
    unsigned char hour_set = b & 0x7F;
    int hour = byte_decode_hour(hour_set);
    if (hour_set != 0x00 && hour < 0) {
        printf("byte 4 wrong %02X\n", hour_set);
        return 1;
    }

    if (hour > 0)
        command->timer_set_hour = hour;

    return 0;
}

int interpret_5(ir_command* command, unsigned char b) {
    // jet turn on / jet is on
    if (b != 0x90 && b != 0x10 && b != 0x00) {
        printf("byte 5 wrong %02X\n", b);
        return 1;
    }

    if (b == 0x90)
        command->jet_state = TURN_ON;
    else if (b == 0x10)
        command->jet_state = ON;

    return 0;
}

int interpret_6(ir_command* command, unsigned char b) {
    // toggle dim
    if (b != 0xA0 && b != 0x80 && b != 0x00) {
        printf("byte 6 wrong %02X\n", b);
        return 1;
    }

    command->is_toggle_dim = (b == 0xA0);
    command->is_around_u_keepalive = (b == 0x00);

    return 0;
}

int interpret_8(ir_command* command, unsigned char b) {
    // toggle swing
    if (b != 0x40 && b != 0x00) {
        printf("byte 8 wrong %02X\n", b);
        return 1;
    }

    command->is_second_toggle_swing = (b == 0x40);

    return 0;
}

int interpret_11(ir_command* command, unsigned char b) {
    // around u on
    if (b != 0x80 && b != 0x00) {
        printf("byte 11 wrong %02X\n", b);
        return 1;
    }

    command->is_around_u_on = (b == 0x80);

    return 0;
}

int interpret_12(ir_command* command, unsigned char b) {
    // 12 is around u temperature
    // if (b != 0x80 && b != 0x00) {
    //     printf("byte 12 wrong %02X\n", b);
    //     return -1;
    // }

    if (b != 0x00)
        command->around_u_temperature = (int)b;

    return 0;
}

int interpret_14(ir_command* command, unsigned char b) {
    // part 1 is sleep modes 2-4
    unsigned char sleep_mode = (b & 0xF0) >> 4;
    switch (sleep_mode) {
        case 0x00:
            break;
        case 0x04:
        case 0x08:
        case 0x0C:
            command->sleep_mode = sleep_mode;
            break;
        default:
            printf("byte 14 part 1 wrong %02X\n", sleep_mode);
            return 1;
    }

    // part 2 is super silent on
    unsigned char super_silent = b & 0x0F;
    if (super_silent != 0x04 && super_silent != 0x00) {
        printf("byte 14 part 2 wrong %02X\n", super_silent);
        return 1;
    }

    command->is_super_silent_on = (super_silent == 0x04);

    return 0;
}

int interpret_15(ir_command* command, unsigned char b) {
    // the operation
    command->op = b;
    return 0;
}

int interpret_16(ir_command* command, unsigned char b) {
    // 6th sense min/max
    if (b != 0x04 && b != 0x00) {
        printf("byte 16 wrong %02X\n", b);
        return 1;
    }

    command->is_sixth_sense_min_or_max = (b == 0x04);

    return 0;
}

int interpret_18(ir_command* command, unsigned char b) {
    // on/off state
    if (b != 0x38 && b != 0x28) {
        printf("byte 18 wrong %02X\n", b);
        return 1;
    }

    command->is_on_state = (b == 0x38);
    return 0;
}

ir_command* interpret(unsigned char* command) {
    // check fixed bytes
    const int standard = command[0] == 0xC1
                      && command[1] == 0x60
                      && command[7] == 0x00
                      && command[9] == 0x00
                      && command[10] == 0x00
                      && command[17] == 0x00
                      && command[19] == 0x00;

    if (!standard) {
        printf("Command does not follow standard convention\n");
        return NULL;
    }

    // 13 and 20 are checksums
    if (command[13] != xor(command + 2, 11) || command[20] != xor(command + 14, 6)) {
        printf("Checksum fail\n");
        return NULL;
    }

    // change endianness to make it more readable
    for (int i = 0; i < IR_COMMAND_LEN; i++)
        command[i] = reverse(command[i]);

    ir_command* comm = (ir_command*)malloc(sizeof(ir_command));
    memset(comm, 0x00, sizeof(ir_command));

    int res = 0;

    res |= interpret_2(comm, command[2]);
    res |= interpret_3(comm, command[3]);
    res |= interpret_4(comm, command[4]);
    res |= interpret_5(comm, command[5]);

    res |= interpret_6(comm, command[6]);
    res |= interpret_8(comm, command[8]);
    res |= interpret_11(comm, command[11]);
    res |= interpret_12(comm, command[12]);

    res |= interpret_14(comm, command[14]);
    res |= interpret_15(comm, command[15]);
    res |= interpret_16(comm, command[16]);
    res |= interpret_18(comm, command[18]);

    if (res) {
        printf("Interpret fail\n");
        return NULL;
    }

    return comm;
}
