#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "serial.h"
#include "interpret.h"
#include "generate.h"
#include "util.h"

void print_command(unsigned char* command, const int* bytes_to_print, int num_bytes, int highlight) {
    for (int i = 0; i < num_bytes; i++) {
        unsigned char b = reverse(command[bytes_to_print[i]]);

        char bin_str[10];
        byte_to_bin_str(bin_str, b);

        if (bytes_to_print[i] == highlight)
            printf("\033[0;31m");

        printf("%s", bin_str);

        if (bytes_to_print[i] == highlight)
            printf("\033[0m");

        if (i < num_bytes - 1)
            printf(" / ");
    }
    printf("\n");

    for (int i = 0; i < num_bytes; i++) {
        unsigned char b = reverse(command[bytes_to_print[i]]);

        if (b == 0x00)
            printf("    .       ");
        else
            printf(" %X    %X     ", b >> 4, b & 0x0F);
    }
    printf("\n");
}

void string_to_binary_command(char* string_comm, unsigned char* byte_comm) {
    int i = 0;
    while (i < IR_COMMAND_LEN) {
        byte_comm[i] = chars_to_byte(string_comm[i*2], string_comm[i*2 + 1]);
        //printf("%02X ", byte_comm[i]);
        i++;
    }
    //printf("\n");
}

int main(int argc, char ** argv) {
    int serial_port = open("/dev/cu.usbmodem1462301", O_RDONLY);
    setup_serial(serial_port);

    while (1) {
        int comm_len = read_length(serial_port);

        char comm[comm_len + 1];
        memset(comm, 0x00, sizeof(comm));
        read_command(serial_port, comm, comm_len);

        // remove spaces and detect any chars that indicate an IR read error
        remove_char(comm, ' ');
        if (strstr(comm, "s") || strstr(comm, "f") || strstr(comm, "x")) {
            printf("Illegal chars: %s\n", comm);
            continue;
        }

        // convert received string into actual bytes
        unsigned char command[IR_COMMAND_LEN];
        string_to_binary_command(comm, command);

        unsigned char cmd[IR_COMMAND_LEN]; // used for testing only
        string_to_binary_command(comm, cmd);

        const int interesting[] = { 2, 3, 4, 5, 6, 8, 11, 12, 14, 15, 16, 18 };
        const int elems = sizeof(interesting)/sizeof(interesting[0]);
        print_command(command, interesting, elems, -1);

        // decode command into struct
        ir_command* ir = interpret(command);
        if (!ir) {
            printf("%s\n", comm);
            exit(1);
        }

        print_ir_command(ir);

        // encode struct into command
        unsigned char* generated = generate(ir);
        // print_command(generated, interesting, elems, -1);

        // compare received command with generated command (based on ir_command struct)
        for (size_t i = 0; i < IR_COMMAND_LEN; i++) {
            if (cmd[i] != generated[i]) {
                printf("different %lu %02X %02X\n", i, reverse(cmd[i]), reverse(generated[i]));
                exit(1);
            }
        }

        free(ir);
        free(generated);
    }

    close(serial_port);
}
