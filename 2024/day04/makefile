EXE := ceressearch
FLAGS := -O3 -Wall -Wextra
SRC := main.c

.phony: all run test clean format

all: $(EXE)

$(EXE): $(SRC)
	gcc $(FLAGS) -o $(EXE) $(SRC)

run: $(EXE)
	./$(EXE) input

test: $(EXE)
	./$(EXE) testinput

clean:
	rm $(EXE)

format: $(SRC)
	clang-format -i --style=Microsoft $(SRC)

