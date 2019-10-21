CC=cc
ifeq ($(VIS),1)
	INC=-DVIS -I../../include/
endif
OMPFLAGS=-homp
CFLAGS=-DROWS=2046 -DCOLS=2046 $(OMPFLAGS) $(INC)
EXE=../../bin/heat_eq_omp
all: heat_eq_omp

heat_eq_omp: heat_eq_omp.o
	@mkdir -p ../../bin
	$(CC) $(CFLAGS) heat_eq_omp.o -o $(EXE)

heat_eq.o: heat_eq_omp.c
		$(CC) -c -o $@ $< $(CFLAGS)

clean:
		rm *.o $(EXE)
cleanall:
		rm *.o *.out *.vtk $(EXE)
