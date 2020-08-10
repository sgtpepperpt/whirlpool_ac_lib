#ifndef __AC_CONTROLLER_H__
#define __AC_CONTROLLER_H__

#include "ir_command.h"

typedef struct {
    int is_on;
    int temperature;
    int sixth_sense_temp;

    int is_timer_on;
    int timer_hours;
    ac_mode_t mode;
    fan_speed_t fan_speed;

    int is_jet_on;
    sleep_mode_t sleep_mode;
    int is_around_u_on;
    int is_super_silent_on;

    // togglers
    // these are not kept as state by the remote, but we track them here
    int swing;
    int dim;
    int wifi;
} ac_state;

void init_state(ac_state* state);
void fill_command(ac_state* state, ir_command* command, unsigned char op);

void turn_on(ac_state* state, ir_command* command);
void turn_off(ac_state* state, ir_command* command);
void set_temp(ac_state* state, ir_command* command, int temperature);
void set_fan(ac_state* state, ir_command* command, fan_speed_t fan_speed);
void set_mode(ac_state* state, ir_command* command, unsigned char mode);

void turn_jet_on(ac_state* state, ir_command* command);
void turn_jet_off(ac_state* state, ir_command* command);

void turn_sixth_sense_on(ac_state* state, ir_command* command);
void set_sixth_temperature(ac_state* state, ir_command* command, int temperature);
void turn_sixth_sense_dehumidify(ac_state* state, ir_command* command);

#endif
