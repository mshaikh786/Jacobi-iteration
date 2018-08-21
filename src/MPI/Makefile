CC=cc
ifeq ($(VIS),1)
	INC=-DVIS -I../../include/
endif
CFLAGS=-DROWS=2000 -DCOLS=2000 $(INC) -g 
LDFLAGS=-dynamic
EXE=heat_eq_mpi
all: heat_eq_mpi

heat_eq_serial: SER/heat_eq_mpi.o
	$(CC) $(CFLAGS) heat_eq_mpi.o -o $(EXE) $(LDFLAGS)

%.o: SER/heat_eq_mpi.c
		$(CC) -c -o $@ $< $(CFLAGS)

clean:
		rm *.o $(EXE)
cleanall:
		rm *.o *.out *.vtk $(EXE)
