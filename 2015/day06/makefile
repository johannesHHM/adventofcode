EXE1 := firehazard1
EXE2 := firehazard2
FLAGS := -O3 -Wall -Wextra
SRC1 := main1.c
SRC2 := main2.c

.phony: all run1 run2 clean format

all: $(EXE1) $(EXE2)

$(EXE1): $(SRC1)
	gcc $(FLAGS) -o $(EXE1) $(SRC1)

$(EXE2): $(SRC2)
	gcc $(FLAGS) -o $(EXE2) $(SRC2)

run1: $(EXE1)
	./$(EXE1) input

run2: $(EXE2)
	./$(EXE2) input

clean:
	rm $(EXE1) $(EXE2)

format: $(SRC)
	clang-format -i --style=Microsoft $(SRC1)	
	clang-format -i --style=Microsoft $(SRC2)

