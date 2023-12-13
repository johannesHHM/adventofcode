EXE1 := camelcards1
EXE2 := camelcards2
FLAGS := -Wall -Wextra

.phony: all run1 run2 clean format

all: $(EXE1) $(EXE2)

$(EXE1): main1.c
	gcc $(FLAGS) -o $(EXE1) main1.c

$(EXE2): main2.c
	gcc $(FLAGS) -o $(EXE2) main2.c

run1: $(EXE1)
	./$(EXE1) input

run2: $(EXE2)
	./$(EXE2) input

clean:
	rm $(EXE1) $(EXE2)

format: main1.c main2.c
	clang-format -i --style=Microsoft main1.c
	clang-format -i --style=Microsoft main2.c

