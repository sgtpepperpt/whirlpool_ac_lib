#include <string.h>

#include "ac_controller.h"

void init_state(ac_state* state) {
    state->is_on = 0;
    state->temperature = 23;
    state->sixth_sense_temp = 0;

    state->is_timer_on = 0;
    state->timer_hours = 0;
    state->mode = COOL;
    state->fan_speed = AUTO;

    state->is_jet_on = 0;
    state->sleep_mode = SLEEP_MODE_1;
    state->is_around_u_on = 0;
    state->is_super_silent_on = 0;

    state->swing = 0;
    state->dim = 0;
    state->wifi = 0;
}

void fill_command(ac_state* state, ir_command* command, unsigned char op) {
    memset(command, 0x00, sizeof(ir_command));

    command->op = op;

    command->temperature = state->temperature;
    command->mode = state->mode;
    command->fan_speed = state->fan_speed;

    command->is_toggle_swing = op == TOGGLE_SWING;
    command->is_second_toggle_swing = op == TOGGLE_SWING;

    command->is_sleep_on = state->sleep_mode? 1 : 0;
    command->sleep_mode = state->sleep_mode;

    command->is_timer_on = state->is_timer_on;
    command->timer_set_hour = command->op == SET_TIMER ? state->timer_hours : 0x00;

    command->is_turn_sixth_sense_on = op == SIXTH_SENSE_TURN_ON;
    if (state->mode == SIXTH_SENSE) {
        switch (state->sixth_sense_temp) {
            case -3:
                command->sixth_sense_temperature = MINUS_MINUS;
                command->is_sixth_sense_min_or_max = 1;
                break;
            case -2:
                command->sixth_sense_temperature = MINUS_MINUS;
                command->is_sixth_sense_min_or_max = 0;
                break;
            case -1:
                command->sixth_sense_temperature = MINUS;
                command->is_sixth_sense_min_or_max = 0;
                break;
            case 0:
                command->sixth_sense_temperature = NEUTRAL;
                command->is_sixth_sense_min_or_max = 0;
                break;
            case 1:
                command->sixth_sense_temperature = PLUS;
                command->is_sixth_sense_min_or_max = 0;
                break;
            case 2:
                command->sixth_sense_temperature = PLUS_PLUS;
                command->is_sixth_sense_min_or_max = 0;
                break;
            case 3:
                command->sixth_sense_temperature = PLUS_PLUS;
                command->is_sixth_sense_min_or_max = 1;
                break;
        }
    } else {
        command->sixth_sense_temperature = 0x00;
        command->is_sixth_sense_min_or_max = 0;
    }

    command->jet_state = (op == TOGGLE_JET && !state->is_jet_on)? TURN_ON : (state->is_jet_on? ON : OFF);
    command->is_toggle_dim = op == TOGGLE_DIM;
    command->is_around_u_keepalive = op == KEEPALIVE_AROUND_U;

    command->is_around_u_on = state->is_around_u_on;
    command->around_u_temperature = 0; // unused; to use send temperature in the command via this param

    command->is_super_silent_on = state->is_super_silent_on;

    command->is_toggle_on_off = op == TURN_ON_OFF;
    command->is_on_state = state->is_on;
}

void turn_on(ac_state* state, ir_command* command) {
    state->is_on = 1;
    fill_command(state, command, TURN_ON_OFF);
}

void turn_off(ac_state* state, ir_command* command) {
    state->is_on = 0;
    fill_command(state, command, TURN_ON_OFF);
}

void set_temp(ac_state* state, ir_command* command, int temperature) {
    if (temperature < MIN_TEMP)
        state->temperature = MIN_TEMP;
    else if (temperature > MAX_TEMP)
        state->temperature = MAX_TEMP;
    else
        state->temperature = temperature;

    fill_command(state, command, CHANGE_TEMP);
}

void set_fan(ac_state* state, ir_command* command, fan_speed_t fan_speed) {
    state->fan_speed = fan_speed;
    fill_command(state, command, CHANGE_FAN);
}

void set_mode(ac_state* state, ir_command* command, unsigned char mode) {
    state->sixth_sense_temp = NEUTRAL;

    if (mode == 'c') {
        state->mode = COOL;
        fill_command(state, command, CHANGE_MODE);
    } else if (mode == 'h') {
        state->mode = HEAT;
        fill_command(state, command, CHANGE_MODE);
    }
}

void turn_jet_on(ac_state* state, ir_command* command) {
    if (state->is_jet_on)
        return;

    fill_command(state, command, TOGGLE_JET);
    state->is_jet_on = 1;
}

void turn_jet_off(ac_state* state, ir_command* command) {
    if (!state->is_jet_on)
        return;

    fill_command(state, command, TOGGLE_JET);
    state->is_jet_on = 0;
}

void turn_sixth_sense_on(ac_state* state, ir_command* command) {
    if (state->mode == SIXTH_SENSE)
        return;

    state->mode = SIXTH_SENSE;
    fill_command(state, command, SIXTH_SENSE_TURN_ON);
}

void set_sixth_temperature(ac_state* state, ir_command* command, int temperature) {
    if (state->mode != SIXTH_SENSE)
        return;

    switch (temperature) {
        case -3:
            state->sixth_sense_temp = MINUS_MINUS;
            break;
        case -2:
            state->sixth_sense_temp = MINUS;
            break;
        case -1:
            state->sixth_sense_temp = MINUS_MINUS;
            break;
        case 0:
            state->sixth_sense_temp = NEUTRAL;
            break;
        case 1:
            state->sixth_sense_temp = MINUS_MINUS;
            break;
        case 2:
            state->sixth_sense_temp = PLUS;
            break;
        case 3:
            state->sixth_sense_temp = PLUS_PLUS;
            break;
    }

    fill_command(state, command, CHANGE_TEMP);
}

void turn_sixth_sense_dehumidify(ac_state* state, ir_command* command) {
    if (state->mode != SIXTH_SENSE)
        return;

    fill_command(state, command, SIXTH_SENSE_DEC_HUM);
}
