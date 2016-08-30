CFLAG = -g -std=c++11 -fmax-errors=3
GTKCFLAG = $(shell pkg-config gtkmm-3.0 --cflags)
GTKLIB = $(shell pkg-config gtkmm-3.0 --libs)
WEBFLAG = $(shell pkg-config webkit2gtk-3.0 --cflags)
WEBLIB = $(shell pkg-config webkit2gtk-3.0 --libs)
CC = g++
SRC = $(wildcard *.cc)
CPP = $(wildcard *.cpp)
GTK = $(wildcard *.c++)
EXE = $(patsubst %.cpp, %.x, $(CPP))
OBJ = $(patsubst %.cc, %.o, $(SRC))
GOBJ = $(patsubst %.c++, %.o, $(GTK))

all : $(OBJ) $(GOBJ) $(EXE) 

%.x : %.cpp $(OBJ) $(GOBJ)
	$(CC) $< -o $@ $(OBJ) $(GOBJ) $(CFLAG) $(GTKCFLAG) $(WEBFLAG) $(GTKLIB) $(WEBLIB)

%.o : %.cc 
	$(CC) $< -c $(CFLAG) $(GTKCFLAG)

%.o : %.c++ %.h
	$(CC) $< -c $(CFLAG) $(GTKCFLAG)

clean :
	rm *.o *.x
