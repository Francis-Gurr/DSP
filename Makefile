# define the name of your source file(s)
<<<<<<< HEAD
SRCS = ./src/main.c ./src/demodulator.c ./src/resample.c ./src/fir_alt.c ./src/io.c ./src/get_lr.c ./src/h/init.c ./src/test/csv.c 
=======
SRCS = ./src/main.c ./src/demodulator.c ./src/resample.c ./src/fir_alt.c ./src/io.c ./src/get_lr.c ./src/h/init.c 
>>>>>>> 9d2363de600188df9f86925610e61cb706258b33

# define the name of the object files(s) - we can do this automatically
OBJS = $(SRCS:.c=.o)

# tell MAKE which compiler to use
CCOMP = gcc

# flags for the compiler
CFLAGS = -Wall -O3 -fstrict-aliasing

# flags for the linker - note -lm for the math library
LDFLAGS = -O3 -lm -L/usr/lib

# the name of your executable file (the target) - here we put it in the top directory
TARGET = main

# actions
all: $(OBJS)
	$(CCOMP) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CCOMP) -c -o $@ $< $(CFLAGS)

# delete all objects and target
clean:
	rm -f $(OBJS) $(TARGET)
