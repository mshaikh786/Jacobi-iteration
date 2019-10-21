CC=cc
ifeq ($(VIS),1)
	INC=-DVIS -I../../include/
endif
CFLAGS=-DROWS=2048 -DCOLS=2048 $(INC)
LDFLAGS=
EXE=../../bin/heat_eq_mpi

mpi: heat_eq_mpi.o
	@mkdir -p ../../bin
	$(CC) $(CFLAGS) heat_eq_mpi.o -o $(EXE) $(LDFLAGS)

heat_eq_mpi.o: heat_eq_mpi.c
		$(CC) -c -o $@ $< $(CFLAGS)

clean:
		rm *.o $(EXE)
cleanall:
		rm *.o *.out *.vtk $(EXE)
