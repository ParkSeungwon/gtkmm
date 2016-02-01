CFLAG = -g -std=c++11 -fmax-errors=3
GTKCFLAG = $(shell pkg-config gtkmm-3.0 --cflags)
GTKLIB = $(shell pkg-config gtkmm-3.0 --libs)
CC = g++
SRC = $(wildcard *.cc)
EXE = $(patsubst %.cc, %, $(SRC))
#POBJ = $(wildcard ../*.o)

all : $(EXE)

#./show

% : %.cc 
	$(CC) $< $(GTKCFLAG) $(GTKLIB) $(CFLAG) -o $@
team : teamsetup.cc
	$(CC) teamsetup.cc $(GTKCFLAG) $(GTKLIB) $(CFLAG) -o $@
	
clean :
	rm *.o
