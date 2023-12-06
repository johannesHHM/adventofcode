EXE := seeds
FLAGS := -O3

.phony: all run clean format

all: $(EXE)

$(EXE): main.c
	gcc $(FLAGS) -o $(EXE) main.c

run: $(EXE)
	./$(EXE) input

clean:
	rm $(EXE)

format: main1.c main2.c
	clang-format -i --style=Microsoft main.c

