package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	if len(os.Args) < 2 {
		println("Error: Name is needed \"go run <filename> <name>\"")
		return
	}
	name := strings.Join(os.Args[1:], " ")
	fmt.Printf("Welcome %v to the jungle \n", name)

}
