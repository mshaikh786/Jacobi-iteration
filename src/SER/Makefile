CC=cc
ifeq ($(VIS),1)
	INC=-DVIS -I../../include/
endif
CFLAGS=-DROWS=2000 -DCOLS=2000 $(INC)
EXE=heat_eq_serial
all: heat_eq_serial

heat_eq_serial: heat_eq.o
	$(CC) $(CFLAGS) heat_eq.o -o $(EXE)

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS)

clean:
		rm *.o $(EXE)
cleanall:
		rm *.o *.out *.vtk $(EXE)
