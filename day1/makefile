EXE := trebuchet
.phony: all run format

all: $(EXE)

$(EXE): main.c
	gcc -o $(EXE) main.c

run: $(EXE)
	./$(EXE) input

clean:
	rm $(EXE)

format: main.c
	clang-format -i --style=Microsoft main.c

