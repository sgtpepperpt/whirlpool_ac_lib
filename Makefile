CFLAGS := -fPIC -Werror
LFLAGS := -lm -lpigpio -pthread
IFLAGS := -I src -I src/emitter -I include

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

libwhirlpoolir.so:
	gcc -c $(CFLAGS) -o ac_controller.o src/emitter/ac_controller.c $(LFLAGS) $(IFLAGS)
	gcc -c $(CFLAGS) -o encode.o src/emitter/encode.c $(LFLAGS) $(IFLAGS)
	gcc -c $(CFLAGS) -o generate.o src/emitter/generate.c $(LFLAGS) $(IFLAGS)
	gcc -c $(CFLAGS) -o util.o src/util.c $(LFLAGS) $(IFLAGS)
	gcc -c $(CFLAGS) -o ir_command.o src/ir_command.c $(LFLAGS) $(IFLAGS)

	gcc -shared -fPIC -Wl,-soname,libwhirlpoolir.so -o libwhirlpoolir.so util.o ir_command.o generate.o ac_controller.o encode.o -lc


install: libwhirlpoolir.so
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 libwhirlpoolir.so $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 include/ac_controller.h $(DESTDIR)$(PREFIX)/include/

clean:
	$(RM) *.o *.so*
