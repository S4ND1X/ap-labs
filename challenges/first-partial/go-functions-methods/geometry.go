// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
)

// Change the Point struct from type Point struct{ X, Y float64 } to type Point struct{ x, y float64 }
type Point struct {
	x, y float64
}

// Auxiliar point
var p0 Point

//Change the Point struct from type Point struct{ X, Y float64 } to type Point struct{ x, y float64 }.

func X(p Point) float64 {
	return p.x
}

func Y(p Point) float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(X(q)-X(p), Y(q)-Y(p))
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(X(q)-X(p), Y(q)-Y(p))
}

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

// Get line orientation
func orientation(p1, p2, p3 Point) int {
	var val float64 = (Y(p2)-Y(p1))*(X(p3)-X(p2)) - (X(p2)-X(p1))*(Y(p3)-Y(p2))

	if val > 0 {
		return 1
	} else if val < 0 {
		return 2
	} else {
		return 0
	}
}

// Compare two points
func compare(p1, p2 *Point) bool {
	var orientation int = orientation(p0, *p1, *p2)
	if orientation == 0 {
		if Distance(p0, *p2) >= Distance(p0, *p1) {
			return false
		} else {
			return true
		}
	} else if orientation == 2 {
		return false
	} else {
		return true
	}
}

// Manual implementation of qsort
func qsort(path Path) Path {
	if len(path) < 2 {
		return path
	}

	left, right := 0, len(path)-1

	// Pick a random pivot
	pivot := rand.Int() % len(path)

	// sqwap the pivot to the right
	path[pivot], path[right] = path[right], path[pivot]

	// Move smaller items to the left
	for i := range path {
		if compare(&path[i], &path[right]) {
			path[i], path[left] = path[left], path[i]
			left++
		}
	}

	// Move pivot to the right of the now partitioned array
	path[left], path[right] = path[right], path[left]

	// Recursively sort the sub-array containing items less than the pivot
	qsort(path[:left])
	qsort(path[left+1:])

	return path
}

// Swap two points
func swap(p1, p2 *Point) {
	var temp Point = *p1
	*p1 = *p2
	*p2 = temp
}

// Find a valid path for the figure without intersections
func findValidPath(path Path, n int) {
	//
	var minY float64 = Y(path[0])
	var min int = 0
	for i := 1; i < n; i++ {
		var y float64 = Y(path[i])
		if (y < minY) || (minY == y && X(path[i]) < X(path[min])) {
			minY = Y(path[i])
			min = i
		}
	}

	// Store reference point and then sort the path
	swap(&path[0], &path[min])
	p0 = path[0]
	qsort(path)

	// Print out the path
	fmt.Printf("- Generating a [%d] sides figure\n", n)
	fmt.Println("- Figure's vertices")
	for i := 0; i < n; i++ {
		fmt.Printf(" - (  %.2f,  %.2f)\n", X(path[i]), Y(path[i]))
	}
	fmt.Println("- Figure's Perimeter")
	fmt.Printf(" - %f", path[0].Distance(path[1]))
	for i := 2; i < n; i++ {
		fmt.Printf(" + %f", path[i-1].Distance(path[i]))
	}
	fmt.Printf(" + %f", path[n-1].Distance(path[0]))
	fmt.Printf(" = %f\n", path.Distance())
}

// Generate random points for all the figure's vertices. Values for vertice's point is in the range (-100,100).
func GeneratePoints(sides int) []Point {
	points := make([]Point, sides)
	for i := range points {
		points[i] = Point{x: 100 * rand.Float64(), y: 100 * rand.Float64()}
	}

	//Make sure that the line segments of your figure doesn't intersect with the previously created lines. In case of intersection, fix it.
	return points
}

// A main function that will allow a cmdline parameter that will be called sides. The sides variable will serve to create an iregular figure.
func main() {

	if len(os.Args) != 2 {
		fmt.Println("Please provide a number of sides.")
		os.Exit(1)
	}

	sides := os.Args[1]
	sidesInt, err := strconv.Atoi(sides)
	if err != nil {
		fmt.Println("Please provide a number of sides.")
		os.Exit(1)
	}

	path := make(Path, sidesInt)
	path = GeneratePoints(sidesInt)

	findValidPath(path, sidesInt)

}

//!-path
