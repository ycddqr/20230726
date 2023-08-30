CC=gcc
BIN = a.out
OBJ = fileoper.o main.o manageroper.o public.o readeroper.o runsever.o superoper.o
FLAG = -Wall -Werror

all:$(OBJ)
	$(CC) $(OBJ) -o $(BIN)
	
%.o:%.c
	$(CC) -c $(FLAG) $< -o $@
	
clean:
	rm -rf $(BIN) $(OBJ)
