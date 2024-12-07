EXE := bridgerepair
FLAGS := -dynamic -no-keep-hi-files -no-keep-o-files
SRC := main.hs

.phony: all run clean

all: $(EXE)

$(EXE): $(SRC)
	ghc $(FLAGS) -o $(EXE) $(SRC)

run: $(EXE)
	./$(EXE) input

clean:
	rm $(EXE)

