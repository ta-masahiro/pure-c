PROGRAM := pure
OBJS := hash.o vector.o symbol.o vm.o object.o lexer.o  parser.o generate.o primitive.o CException.o array.o

CC := gcc
#FLAGS := -g -O0  -DDEBUG
FLAGS := -O3
#LDLIBS :=  -lgc -lgmp -lmpfr -lm  -lecm
LDLIBS :=  -lgc -lgmp -lmpfr -lm -lblas  -llapacke  -lecm

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(FLAGS) $^ $(LDLIBS)

$(OBJS): general.h hash.h vector.h vm.h object.h symbol.h lexparse.h generate.h array.h

.SUFFIXES: .c .o
.c.o:
	$(CC) $(FLAGS) -c $<
clean:
	$(RM) *.o
	$(RM) pure
