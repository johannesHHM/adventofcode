package main

import (
	"fmt"
	"os"
)

func deliverPresents(instructions string) {
	var houseMap = make(map[string]int)
	var sled int = 0
	x := [2]int{0, 0}
	y := [2]int{0, 0}

	houseMap["0x0"] += 2

	for _, command := range instructions {
		switch command {
		case '^':
			y[sled]++
		case 'v':
			y[sled]--
		case '>':
			x[sled]++
		case '<':
			x[sled]--
		default:
			fmt.Println("Unknow command found!")
		}

		houseMap[fmt.Sprintf("%dx%d", x[sled], y[sled])]++
		sled = (sled + 1) % 2
	}

	fmt.Printf("Houses that got presents: %d\n", len(houseMap))
}

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Input file path required.")
		os.Exit(0)
	}

	input, err := os.ReadFile(os.Args[1])
	if err != nil {
		fmt.Print(err)
		os.Exit(1)
	}

	deliverPresents(string(input))
}
