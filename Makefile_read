PROGRAM := vm-test
OBJS := hash.o vector.o read.o vm.o object.o symbol.o

CC := gcc
#FLAGS := -g -O0
FLAGS := -O3
LDLIBS :=  -lgc -lgmp -lm

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(FLAGS) $^ $(LDLIBS)

$(OBJS): general.h hash.h vector.h vm.h object.h symbol.h

.SUFFIXES: .c .o
.c.o:
	$(CC) $(FLAGS) -c $<
clean:
	$(RM) *.o
	$(RM) vm-test
