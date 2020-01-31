# define the name of your source file(s)
SRCS = ./src/main.c ./src/demodulator.c ./src/resample.c ./src/fir.c ./src/io.c ./src/get_lr.c ./src/h/init.c ./src/test/csv.c 

# define the name of the object files(s) - we can do this automatically
OBJS = $(SRCS:.c=.o)

# tell MAKE which compiler to use
CCOMP = gcc

# flags for the compiler
CFLAGS = -Wall -fopenmp -O3 -fstrict-aliasing

# flags for the linker - note -lm for the math library
LDFLAGS = -O3 -fopenmp -L/usr/lib

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
