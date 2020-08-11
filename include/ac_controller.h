#ifndef __AC_CONTROLLER_H__
#define __AC_CONTROLLER_H__

#define MIN_TEMP 16
#define MAX_TEMP 30

typedef enum {
    AC_MODE_COOL = 0x02,
    AC_MODE_HEAT = 0x00,
    AC_MODE_FAN = 0x04,
    AC_MODE_SIXTH_SENSE = 0x01
} ac_mode_t;

typedef enum {
    AC_FAN_AUTO = 0x00,
    AC_FAN_MIN_OR_SUPER_SILENT = 0x03,
    AC_FAN_MED = 0x02,
    AC_FAN_MAX = 0x01
} fan_speed_t;

typedef enum {
    AC_SLEEP_MODE_1 = 0x00,
    AC_SLEEP_MODE_2 = 0x04,
    AC_SLEEP_MODE_3 = 0x08,
    AC_SLEEP_MODE_4 = 0x0C
} sleep_mode_t;

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

void turn_on(ac_state* state);
void turn_off(ac_state* state);
void set_temp(ac_state* state, int temperature);
void set_fan(ac_state* state, fan_speed_t fan_speed);
void set_mode(ac_state* state, char mode);

void turn_jet_on(ac_state* state);
void turn_jet_off(ac_state* state);

void turn_sixth_sense_on(ac_state* state);
void set_sixth_temperature(ac_state* state, int temperature);
void turn_sixth_sense_dehumidify(ac_state* state);

#endif
