CC=cc
ifeq ($(VIS),1)
	INC=-DVIS -Iinclude/
endif
CFLAGS=-homp -h msglevel_4 $(INC)
SRC=./src
BIN=./bin
all: make_bin serial omp mpi

make_bin:
	@mkdir -p $(BIN)

serial: heat_serial.o
	$(CC) $(CFLAGS) $(SRC)/SER/heat_serial.o -o $(BIN)/heat_$@

heat_serial.o: $(SRC)/SER/heat_serial.c
	$(CC) $(CFLAGS) -c $< -o $(SRC)/SER/$@


omp: heat_omp.o
	$(CC) $(CFLAGS) $(SRC)/OMP/heat_omp.o -o $(BIN)/heat_$@

heat_omp.o: $(SRC)/OMP/heat_omp.c
	$(CC) $(CFLAGS) -c $< -o $(SRC)/OMP/$@


mpi: heat_mpi.o
	$(CC) $(CFLAGS) $(SRC)/MPI/heat_mpi.o -o $(BIN)/heat_$@

heat_mpi.o: $(SRC)/MPI/heat_mpi.c
		$(CC) $(CFLAGS) -c $< -o $(SRC)/MPI/$@


clean: clean_serial clean_omp clean_mpi

clean_serial:
	rm $(SRC)/SER/*.o bin/heat_serial
clean_omp:
	rm $(SRC)/OMP/*.o bin/heat_omp
clean_mpi:
	rm $(SRC)/MPI/*.o bin/heat_mpi
