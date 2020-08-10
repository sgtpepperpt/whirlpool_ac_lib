#ifndef __SERIAL_H__
#define __SERIAL_H__

void setup_serial(int serial_port);
int read_length(int serial_port);
void read_command(int serial_port, char* comm, int comm_len);

#endif
