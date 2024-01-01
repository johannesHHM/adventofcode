EXE := lavalagoon
FLAGS := -O3 -Wall -Wextra
SRC := main.c

.phony: all run clean format

all: $(EXE)

$(EXE): $(SRC)
	gcc $(FLAGS) -o $(EXE) $(SRC)

run: $(EXE)
	./$(EXE) input

clean:
	rm $(EXE)

format: $(SRC)
	clang-format -i --style=Microsoft $(SRC)

