# Whirlpool A/C driver library
This C library converts high-level A/C commands into IR strings to control a Whirlpool SPIW312A2WF A/C unit.
Available functions are on `include/ac_controller.h`. An example can be found on `src/emitter/example.c`.

The code also includes a receiver program, used to help decode the remote controller codes. If you only want to
integrate the A/C controller into your program you can compile only the library module.

## Dependencies
* [IR Slinger](https://github.com/bschwind/ir-slinger)

## Compiling

To compile as a shared library just run ```make && sudo make install```

To compile the decoder/receiver run ```make decoder```

To compile the emitter example run ```make example```
