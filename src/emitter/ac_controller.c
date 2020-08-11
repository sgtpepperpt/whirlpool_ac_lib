#include <string.h>

#include "encode.h"
#include "ir_command.h"

#include "ac_controller.h"

raw_mode_t get_raw_mode(ac_mode_t mode) {
    switch (mode) {
        case AC_MODE_COOL:
            return COOL;
        case AC_MODE_HEAT:
            return HEAT;
        case AC_MODE_FAN:
            return FAN;
        case AC_MODE_SIXTH_SENSE:
            return SIXTH_SENSE;
    }
}

raw_fan_speed_t get_raw_fan_speed(fan_speed_t fan_speed) {
    switch (fan_speed) {
        case AC_FAN_AUTO:
            return AUTO;
        case AC_FAN_MIN_OR_SUPER_SILENT:
            return MIN_OR_SUPER_SILENT;
        case AC_FAN_MED:
            return MED;
        case AC_FAN_MAX:
            return MAX;
    }
}

raw_sleep_mode_t get_raw_sleep_mode(sleep_mode_t sleep_mode) {
    switch (sleep_mode) {
        case AC_SLEEP_MODE_1:
            return SLEEP_MODE_1;
        case AC_SLEEP_MODE_2:
            return SLEEP_MODE_2;
        case AC_SLEEP_MODE_3:
            return SLEEP_MODE_3;
        case AC_SLEEP_MODE_4:
            return SLEEP_MODE_4;
    }
}

void init_state(ac_state* state) {
    state->is_on = 0;
    state->temperature = 23;
    state->sixth_sense_temp = 0;

    state->is_timer_on = 0;
    state->timer_hours = 0;
    state->mode = COOL;
    state->fan_speed = AUTO;

    state->is_jet_on = 0;
    state->sleep_mode = AC_SLEEP_MODE_1;
    state->is_around_u_on = 0;
    state->is_super_silent_on = 0;

    state->swing = 0;
    state->dim = 0;
    state->wifi = 0;
}

void create_command_and_send(ac_state* state, unsigned char op) {
    ir_command command;
    memset(&command, 0x00, sizeof(&command));

    command.op = op;

    command.temperature = state->temperature;
    command.mode = get_raw_mode(state->mode);
    command.fan_speed = get_raw_fan_speed(state->fan_speed);

    command.is_toggle_swing = op == TOGGLE_SWING;
    command.is_second_toggle_swing = op == TOGGLE_SWING;

    command.is_sleep_on = state->sleep_mode? 1 : 0;
    command.sleep_mode = get_raw_sleep_mode(state->sleep_mode);

    command.is_timer_on = state->is_timer_on;
    command.timer_set_hour = command.op == SET_TIMER ? state->timer_hours : 0x00;

    command.is_turn_sixth_sense_on = op == SIXTH_SENSE_TURN_ON;
    if (state->mode == AC_MODE_SIXTH_SENSE) {
        switch (state->sixth_sense_temp) {
            case -3:
                command.sixth_sense_temperature = MINUS_MINUS;
                command.is_sixth_sense_min_or_max = 1;
                break;
            case -2:
                command.sixth_sense_temperature = MINUS_MINUS;
                command.is_sixth_sense_min_or_max = 0;
                break;
            case -1:
                command.sixth_sense_temperature = MINUS;
                command.is_sixth_sense_min_or_max = 0;
                break;
            case 0:
                command.sixth_sense_temperature = NEUTRAL;
                command.is_sixth_sense_min_or_max = 0;
                break;
            case 1:
                command.sixth_sense_temperature = PLUS;
                command.is_sixth_sense_min_or_max = 0;
                break;
            case 2:
                command.sixth_sense_temperature = PLUS_PLUS;
                command.is_sixth_sense_min_or_max = 0;
                break;
            case 3:
                command.sixth_sense_temperature = PLUS_PLUS;
                command.is_sixth_sense_min_or_max = 1;
                break;
        }
    } else {
        command.sixth_sense_temperature = 0x00;
        command.is_sixth_sense_min_or_max = 0;
    }

    command.jet_state = (op == TOGGLE_JET && !state->is_jet_on)? TURN_ON : (state->is_jet_on? ON : OFF);
    command.is_toggle_dim = op == TOGGLE_DIM;
    command.is_around_u_keepalive = op == KEEPALIVE_AROUND_U;

    command.is_around_u_on = state->is_around_u_on;
    command.around_u_temperature = 0; // unused; to use send temperature in the command via this param

    command.is_super_silent_on = state->is_super_silent_on;

    command.is_toggle_on_off = op == TURN_ON_OFF;
    command.is_on_state = state->is_on;

    // send the generated command via IR
    ir_send(&command);
}

void turn_on(ac_state* state) {
    state->is_on = 1;
    create_command_and_send(state, TURN_ON_OFF);
}

void turn_off(ac_state* state) {
    state->is_on = 0;
    create_command_and_send(state, TURN_ON_OFF);
}

void set_temp(ac_state* state, int temperature) {
    if (temperature < MIN_TEMP)
        state->temperature = MIN_TEMP;
    else if (temperature > MAX_TEMP)
        state->temperature = MAX_TEMP;
    else
        state->temperature = temperature;

    create_command_and_send(state, CHANGE_TEMP);
}

void set_fan(ac_state* state, fan_speed_t fan_speed) {
    state->fan_speed = fan_speed;
    create_command_and_send(state, CHANGE_FAN);
}

void set_mode(ac_state* state, char mode) {
    state->sixth_sense_temp = NEUTRAL;
    state->mode = mode;
    create_command_and_send(state, CHANGE_MODE);
}

void turn_jet_on(ac_state* state) {
    if (state->is_jet_on)
        return;

    if (state->mode == AC_MODE_COOL)
        state->temperature = MIN_TEMP;
    else if (state->mode == AC_MODE_HEAT)
        state->temperature = MAX_TEMP;

    create_command_and_send(state, TOGGLE_JET);
    state->is_jet_on = 1;
}

void turn_jet_off(ac_state* state) {
    if (!state->is_jet_on)
        return;

    state->temperature = 23;
    create_command_and_send(state, TOGGLE_JET);
    state->is_jet_on = 0;
}

void turn_sixth_sense_on(ac_state* state) {
    if (state->mode == AC_MODE_SIXTH_SENSE)
        return;

    state->mode = AC_MODE_SIXTH_SENSE;
    create_command_and_send(state, SIXTH_SENSE_TURN_ON);
}

void set_sixth_temperature(ac_state* state, int temperature) {
    if (state->mode != AC_MODE_SIXTH_SENSE)
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

    create_command_and_send(state, CHANGE_TEMP);
}

void turn_sixth_sense_dehumidify(ac_state* state) {
    if (state->mode != AC_MODE_SIXTH_SENSE)
        return;

    create_command_and_send(state, SIXTH_SENSE_DEC_HUM);
}
