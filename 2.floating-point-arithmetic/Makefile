CC=g++
CFLAG=-Wall -Werror -g
SRC=$(wildcard *.cc)
HDR=$(wildcard *.h)
OBJ=$(SRC:.cc=.o)
EXE=float

.PHONY: clean

$(EXE): $(OBJ)
	$(CC) $(CFLAG) -o $@ $(OBJ)

%.o: %.cc $(HDR)
	$(CC) $(CFLAG) -o $@ -c $<

clean:
	rm -f $(OBJ) $(EXE)

