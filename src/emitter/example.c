#include <stdlib.h>
#include <unistd.h>

#include "encode.h"
#include "ac_controller.h"
#include "ir_command.h"

int main(int argc, char ** argv) {
    ac_state state;
    init_state(&state);

    // generate command to set temp
    ir_command ir;
    turn_on(&state, &ir);
    ir_send(&ir);
    sleep(8);

    set_temp(&state, &ir, 16);
    ir_send(&ir);
    sleep(8);

    set_temp(&state, &ir, 23);
    ir_send(&ir);
    sleep(8);

    turn_sixth_sense_on(&state, &ir);
    ir_send(&ir);
    sleep(8);

    turn_sixth_sense_dehumidify(&state, &ir);
    ir_send(&ir);
    sleep(8);

    turn_off(&state, &ir);
    ir_send(&ir);

    return 0;
}
