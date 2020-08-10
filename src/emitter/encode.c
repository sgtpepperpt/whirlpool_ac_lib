#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "irslinger.h"
#include "encode.h"
#include "generate.h"

#define GPIO_PIN 13
#define PRINT 0

const int ir_start[] = { 9000, 4500 };
const int ir_pause[] = { 500, 8000 };
const int ir_zero[] = { 560, 560 };
const int ir_one[] = { 560, 1680 };

int* encode_command(unsigned char* command, int* pulse_size) {
    int* encoded = (int*)malloc(1024 * sizeof(int));
    memset(encoded, 0x00, sizeof(1024 * sizeof(int)));

    int enc_pos = 0;

    // signal start
    encoded[enc_pos++] = ir_start[0];
    encoded[enc_pos++] = ir_start[1];

    for (size_t i = 0; i < IR_COMMAND_LEN; i++) {
        if (i == 6 || i == 14) {
            encoded[enc_pos++] = ir_pause[0];
            encoded[enc_pos++] = ir_pause[1];
        }

        for (int c = 7; c >= 0; c--) {
            int bit = command[i] >> c;

            if (bit & 1) {
                encoded[enc_pos++] = ir_one[0];
                encoded[enc_pos++] = ir_one[1];
            } else {
                encoded[enc_pos++] = ir_zero[0];
                encoded[enc_pos++] = ir_zero[1];
            }
        }
    }

    // signal end
    encoded[enc_pos++] = 560;

    *pulse_size = enc_pos;

    return encoded;
}

void ir_send(ir_command* ir) {
    unsigned char* generated = generate(ir);

    // get its interpretation so we can compare it with a "real" command
    #if PRINT
        print_ir_command(ir);
    #endif

    // encode into pulses and send
    int pulse_size = 0;
    int* encoded_command = encode_command(generated, &pulse_size);
    int result = irSlingRaw(GPIO_PIN, 38000, 0.5, encoded_command, pulse_size);

    // char generated_cmd[43];
    // for (size_t i = 0; i < IR_COMMAND_LEN; i++)
    //     sprintf(generated_cmd + i * 2, "%02X", generated[i]);
    // generated_cmd[42] = 0x00;
    // printf("%s\n", generated_cmd);

    free(generated);
    free(encoded_command);
}
