#ifndef __IR_COMMAND_H__
#define __IR_COMMAND_H__

#define IR_COMMAND_LEN 21

typedef enum {
    PLUS_PLUS = 0x02,
    PLUS = 0x01,
    NEUTRAL = 0x00,
    MINUS = 0x05,
    MINUS_MINUS = 0x06
} sixth_sense_temp;

typedef enum {
    AUTO = 0x00,
    MIN_OR_SUPER_SILENT = 0x03,
    MED = 0x02,
    MAX = 0x01
} raw_fan_speed_t;

typedef enum {
    COOL = 0x02,
    HEAT = 0x00,
    FAN = 0x04,
    SIXTH_SENSE = 0x01
} raw_mode_t;

typedef enum {
    OFF = 0x00,
    TURN_ON = 0x90,
    ON = 0x10
} jet_state_t;

typedef enum {
    SLEEP_MODE_1 = 0x00,
    SLEEP_MODE_2 = 0x04,
    SLEEP_MODE_3 = 0x08,
    SLEEP_MODE_4 = 0x0C
} raw_sleep_mode_t;

typedef enum {
    CHANGE_TEMP = 0x02,
    CHANGE_MODE = 0x06,
    CHANGE_FAN = 0x11,
    SIXTH_SENSE_TURN_ON = 0x17,
    // SIXTH_SENSE_TURN_OFF = CHANGE_MODE,
    SIXTH_SENSE_DEC_HUM = 0x35,
    SIXTH_SENSE_INC_HUM = 0x34,
    // SIXTH_SENSE_CHANGE_TEMP = CHANGE_TEMP,
    TOGGLE_JET = 0x04,
    SET_TIMER = 0x05,
    CHANGE_SLEEP = 0x03,
    TOGGLE_SWING = 0x07,
    TOGGLE_DIM = 0x00,
    TOGGLE_AROUND_U = 0x0D,
    TOGGLE_SUPER_SILENT = 0x0B,
    TOGGLE_WIFI = 0x32,
    TURN_ON_OFF = 0x01,
    KEEPALIVE_AROUND_U = 0x00
} operations;

typedef struct {
    // byte 2
    int is_toggle_swing;
    sixth_sense_temp sixth_sense_temperature;
    int is_sleep_on;
    int is_toggle_on_off;
    raw_fan_speed_t fan_speed;

    // byte 3
    int temperature;
    int is_timer_on;
    raw_mode_t mode;

    // byte 4
    int timer_set_hour;
    int is_turn_sixth_sense_on;

    // byte 5
    jet_state_t jet_state;

    // byte 6
    int is_toggle_dim;
    int is_around_u_keepalive;

    // byte 8
    int is_second_toggle_swing;

    // byte 11
    int is_around_u_on;

    // byte 12
    int around_u_temperature;

    // byte 14
    raw_sleep_mode_t sleep_mode;
    int is_super_silent_on;

    // byte 15
    operations op;

    // byte 16
    int is_sixth_sense_min_or_max;

    // byte 18
    int is_on_state;
} ir_command;

void print_ir_command(ir_command* command);

#endif
