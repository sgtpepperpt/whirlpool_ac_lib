#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "generate.h"
#include "ac_controller.h"
#include "util.h"

unsigned char byte_encode_hour(int hour) {
    const int timer_hour_vals[24] = { 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22 };

    return timer_hour_vals[hour - 1];
}

unsigned char generate_2(ir_command* command) {
    unsigned char b = 0x00;

    // bit 0 is toggle swing
    b |= command->is_toggle_swing << 7;

    // bit 1-3 is sixth sense temp
    b |= command->sixth_sense_temperature << 4;

    // bit 4 is sleep mode on
    b |= command->is_sleep_on << 3;

    // bit 5 is toggle on/off
    b |= command->is_toggle_on_off << 2;

    // bits 6-7 is fan speed
    b |= command->fan_speed;

    return b;
}

unsigned char generate_3(ir_command* command) {
    unsigned char b = 0x00;

    // 4 bits for temp
    b |= (command->temperature - MIN_TEMP) << 4;

    // 1 bit if is timer on
    b |= command->is_timer_on << 3;

    // 3 bits for mode
    b |= command->mode;

    return b;
}

unsigned char generate_4(ir_command* command) {
    unsigned char b = 0x00;

    // first bit is turn on 6th sense
    b |= command->is_turn_sixth_sense_on << 7;

    // the rest is timer set hour
    b |= byte_encode_hour(command->timer_set_hour) & 0x7F;

    return b;
}

unsigned char generate_5(ir_command* command) {
    return command->jet_state;
}

unsigned char generate_6(ir_command* command) {
    if (command->is_toggle_dim)
        return 0xA0;
    else if (command->is_around_u_keepalive)
        return 0x00;
    else
        return 0x80;
}

unsigned char generate_8(ir_command* command) {
    return command->is_second_toggle_swing? 0x40 : 0x00;
}

unsigned char generate_11(ir_command* command) {
    return command->is_around_u_on? 0x80 : 0x00;
}

unsigned char generate_12(ir_command* command) {
    return (unsigned char)command->around_u_temperature;
}

unsigned char generate_14(ir_command* command) {
    unsigned char b = 0x00;

    // part 1 is sleep modes 2-4
    b |= command->sleep_mode << 4;

    // part 2 is super silent on
    if (command->is_super_silent_on)
        b |= 0x04;

    return b;
}

unsigned char generate_15(ir_command* command) {
    return command->op;
}

unsigned char generate_16(ir_command* command) {
    if(command->is_sixth_sense_min_or_max)
        return 0x04;
    else
        return 0x00;
}

unsigned char generate_18(ir_command* command) {
    if (command->is_on_state)
        return 0x38;
    else
        return 0x28;
}

unsigned char* generate(ir_command* command) {
    unsigned char* comm = malloc(IR_COMMAND_LEN);
    memset(comm, 0x00, IR_COMMAND_LEN);

    comm[2] = generate_2(command);
    comm[3] = generate_3(command);
    comm[4] = generate_4(command);
    comm[5] = generate_5(command);

    comm[6] = generate_6(command);
    comm[8] = generate_8(command);
    comm[11] = generate_11(command);
    comm[12] = generate_12(command);

    comm[14] = generate_14(command);
    comm[15] = generate_15(command);
    comm[16] = generate_16(command);
    comm[18] = generate_18(command);

    for (int i = 0; i < IR_COMMAND_LEN; i++)
        comm[i] = reverse(comm[i]);

    // generate checksums
    comm[13] = xor(comm + 2, 11);
    comm[20] = xor(comm + 14, 6);

    // add header
    comm[0] = 0xC1;
    comm[1] = 0x60;

    return comm;
}
