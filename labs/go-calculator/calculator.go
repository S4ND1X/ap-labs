package main

import (
	"fmt"
	"os"
	"strconv"
)

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
func calc(operator int, values []int) int {

	result := 0

	if operator == 3 {
		result = 1
	} else {
		result = 0
	}

	for _, value := range values {
		if operator == 1 {
			result += value
		} else if operator == 2 {
			result -= value
		} else if operator == 3 {
			result *= value
		}
	}

	return result

}

func getOperator(operator string) int {
	if operator == "add" {
		return 1
	} else if operator == "sub" {
		return 2
	} else if operator == "mult" {
		return 3
	}
	return 0
}

func main() {

	// get the command line arguments
	args := os.Args[1:]

	// if there is only one argument, print an error and exit
	if len(args) < 3 {
		fmt.Println("Error: Not enough arguments...")
		os.Exit(1)
	}

	// get the operator
	operator := args[0]

	// get the values
	values := args[1:]

	operatorInt := getOperator(operator)

	if operatorInt == 0 {
		fmt.Println("Error: Invalid operator...")
		os.Exit(1)
	}

	valuesInt := []int{}
	// if the values are not all integers, print an error and exit
	for _, value := range values {
		val, err := strconv.Atoi(value)
		if err != nil {
			fmt.Println("Error: Not all values are integers...")
			os.Exit(1)
		} else {
			valuesInt = append(valuesInt, val)
		}
	}

	// calculate the result
	result := calc(operatorInt, valuesInt)

	// print the result
	fmt.Println(result)

}
