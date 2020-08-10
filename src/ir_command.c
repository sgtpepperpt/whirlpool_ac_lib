#include <stdio.h>
#include <string.h>

#include "ir_command.h"

void print_ir_command(ir_command* command) {
    char state[1024];
    memset(state, 0x00, sizeof(char) * 1024);

    // byte 2
    if (command->is_toggle_swing)
        strcat(state, "toggling swing; ");

    switch (command->sixth_sense_temperature) {
        case PLUS_PLUS:
            strcat(state, "6th temp +2/+3; ");
            break;
        case PLUS:
            strcat(state, "6th temp +1; ");
            break;
        case NEUTRAL:
            strcat(state, "6th temp 0; ");
            break;
        case MINUS:
            strcat(state, "6th temp -1; ");
            break;
        case MINUS_MINUS:
            strcat(state, "6th temp -2/-3; ");
            break;
    }

    if (command->is_sleep_on)
        strcat(state, "sleep is on; ");

    if (command->is_toggle_on_off)
        strcat(state, "toggling on/off state; ");

    switch (command->fan_speed) {
        case AUTO:
            strcat(state, "fan auto; ");
            break;
        case MED:
            strcat(state, "fan med; ");
            break;
        case MAX:
            strcat(state, "fan high; ");
            break;
        case MIN_OR_SUPER_SILENT:
            strcat(state, "fan low / super silent; ");
            break;
    }

    // byte 3
    char s1[32];
    snprintf(s1, 32, "temp %d; ", command->temperature);
    strcat(state, s1);

    if (command->is_timer_on)
        strcat(state, "timer is on; ");

    switch (command->mode) {
        case SIXTH_SENSE:
            strcat(state, "mode 6th sense; ");
            break;
        case HEAT:
            strcat(state, "mode heat; ");
            break;
        case COOL:
            strcat(state, "mode cool; ");
            break;
        case FAN:
            strcat(state, "mode fan; ");
            break;
    }

    // byte 4
    if (command->timer_set_hour) {
        snprintf(s1, 32, "set timer to %d; ", command->timer_set_hour);
        strcat(state , s1);
    }

    if (command->is_turn_sixth_sense_on)
        strcat(state, "turn on 6th sense; ");

    // byte 5
    if (command->jet_state == TURN_ON)
        strcat(state, "turn on jet; ");
    else if (command->jet_state == ON)
        strcat(state, "jet is on; ");

    // byte 6
    if (command->is_toggle_dim)
        strcat(state, "toggle dim; ");

    if (command->is_around_u_keepalive)
        strcat(state, "keepalive around u; ");

    // byte 8
    if (command->is_second_toggle_swing)
        strcat(state, "toggle swing (2nd); ");

    // byte 11
    if (command->is_around_u_on)
        strcat(state, "around u is on; ");

    // byte 12
    if (command->around_u_temperature) {
        snprintf(s1, 32, "around u temp %d; ", command->around_u_temperature);
        strcat(state, s1);
    }

    // byte 14
    switch (command->sleep_mode) {
        case SLEEP_MODE_1: break; // either is mode 1 or sleep off
        case SLEEP_MODE_2: strcat(state, "sleep mode 2; "); break;
        case SLEEP_MODE_3: strcat(state, "sleep mode 3; "); break;
        case SLEEP_MODE_4: strcat(state, "sleep mode 4; "); break;
    }

    if (command->is_super_silent_on)
        strcat(state, "super silent is on; ");

    // byte 15
    switch (command->op) {
        case CHANGE_TEMP:
            strcat(state, "op change temp; ");
            break;
        case CHANGE_MODE:
            strcat(state, "op change mode; ");
            break;
        case CHANGE_FAN:
            strcat(state, "op change fan; ");
            break;
        case SIXTH_SENSE_TURN_ON:
            strcat(state, "op 6th sense on; ");
            break;
        case SIXTH_SENSE_DEC_HUM:
            strcat(state, "op 6th sense hum-; ");
            break;
        case SIXTH_SENSE_INC_HUM:
            strcat(state, "op 6th sense hum+; ");
            break;
        case TOGGLE_JET:
            strcat(state, "op toggle jet; ");
            break;
        case SET_TIMER:
            strcat(state, "op set timer; ");
            break;
        case CHANGE_SLEEP:
            strcat(state, "op change sleep; ");
            break;
        case TOGGLE_SWING:
            strcat(state, "op toggle swing; ");
            break;
        case TOGGLE_DIM:
            strcat(state, "op toggle dim / keepalive around u; ");
            break;
        case TOGGLE_AROUND_U:
            strcat(state, "op toggle around u; ");
            break;
        case TOGGLE_SUPER_SILENT:
            strcat(state, "op toggle super silent; ");
            break;
        case TOGGLE_WIFI:
            strcat(state, "op toggle wifi; ");
            break;
        case TURN_ON_OFF:
            strcat(state, "op turn on/off; ");
            break;
    }

    // byte 16
    if (command->is_sixth_sense_min_or_max)
        strcat(state, "6th sense in min or max; ");

    // byte 18
    if (command->is_on_state)
        strcat(state, "on state; ");
    else
        strcat(state, "off state; ");

    printf("%s\n", state);
}
