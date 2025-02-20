CC=icx
CFLAGS=-O2 -g -Wall -march=native -ffast-math -funroll-loops -ftree-vectorize
OPTFLAGS=-O3 -fopenmp -g -Wall -march=native -ffast-math -funroll-loops -ftree-vectorize
OBJS_COMMON=kernel.o rdtsc.o

all:	check calibrate measure

check:	$(OBJS_COMMON) driver_check.o
	$(CC) -o $@ $^ -lm -fopenmp

calibrate: $(OBJS_COMMON) driver_calib.o
	$(CC) -o $@ $^ -lm -fopenmp

measure: $(OBJS_COMMON) driver.o
	$(CC) -o $@ $^ -lm -fopenmp

driver_check.o: driver_check.c
	$(CC) $(CFLAGS) -D CHECK -c $< -o $@

driver_calib.o: driver_calib.c
	$(CC) $(CFLAGS) -D CALIB -c $< -o $@

driver.o: driver.c
	$(CC) $(CFLAGS) -c $<

kernel.o: kernel.c
	$(CC) $(OPTFLAGS) $(if $(OPT),-D $(OPT)) -c $< -o $@

clean:
	rm -rf $(OBJS_COMMON) driver_check.o driver_calib.o driver.o check calibrate measure

