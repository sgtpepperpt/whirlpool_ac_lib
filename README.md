The emitter uses [IR Slinger](https://github.com/bschwind/ir-slinger) by Brian Schwind.

```
gcc -o decode src/receiver/*.c src/*.c src/emitter/generate.c -I src -I src/emitter
```

```
gcc -o encode src/emitter/*.c src/*.c -I src -I src/emitter -lm -lpigpio -pthread
```

To compile as library do ```make && sudo make install```.
