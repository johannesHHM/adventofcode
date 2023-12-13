EXE := calcwrapp
.phony: all run clean format

all: $(EXE)

$(EXE): main.c
	gcc -o $(EXE) main.c

run: $(EXE)
	./$(EXE) input

clean:
	rm $(EXE)

format: main.c
	clang-format -i --style=Microsoft main.c

