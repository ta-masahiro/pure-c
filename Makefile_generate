PROGRAM := pure
OBJS := hash.o vector.o symbol.o vm.o object.o lexer.o  parser.o generate.o primitive.o CException.o

CC := gcc
FLAGS := -g -O0
#FLAGS := -O3
LDLIBS :=  -lgc -lgmp -lmpfr -lm

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(FLAGS) $^ $(LDLIBS)

$(OBJS): general.h hash.h vector.h vm.h object.h symbol.h lexparse.h generate.h

.SUFFIXES: .c .o
.c.o:
	$(CC) $(FLAGS) -c $<
clean:
	$(RM) *.o
	$(RM) pure
