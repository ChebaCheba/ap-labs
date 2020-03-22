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
	"time"
)

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

//!-point
func On_Segment(p, q, r Point) bool {
	if (((q.x <= math.Max(p.x, r.x)) && (q.x>=math.Min(p.x, r.x))) && ((q.y <= math.Max(p.y, r.y)) && (q.y>=math.Min(p.y, r.y)))) {
		return true
	}
	return false
}

func Orientation(p, q, r Point) int {
	or := float64(((q.y - p.y) * (r.x - q.x))-((q.x - p.x) * (r.y - q.y)))
	if or > 0 {
		return 1
	} else if or < 0 {  
		return 2
	} else {  
		return 0
	}
}

func Intersect(p1, q1, q2, p2  Point) bool {
	o1 := Orientation(p1, q1, p2) 
    o2 := Orientation(p1, q1, q2) 
    o3 := Orientation(p2, q2, p1) 
	o4 := Orientation(p2, q2, q1)
	
	if ((o1 != o2) && (o3!=o4)) {
		return true
	}

	if ((o1 == 0) && On_Segment(p1, p2, q1)) { 
		return true
	}
   
    if ((o2 == 0) && On_Segment(p1, q2, q1)) { 
		return true
	}
     
    if ((o3 == 0) && On_Segment(p2, p1, q2)) { 
		return true
	}
     
    if ((o4 == 0) && On_Segment(p2, q1, q2)) { 
		return true
	} 
    return false
}

//!+path
func Generate_Point() Point {
	rand.Seed(time.Now().UnixNano())
	x := float64(rand.Intn(200)-rand.Intn(200))/2
	y := float64(rand.Intn(200)-rand.Intn(200))/2
	return Point{x, y}
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

func Generate_Path(path []Point, sides int) []Point {
	point1 := Generate_Point()
	point2 := Generate_Point()
	for (point1.x==point2.x) && (point1.y==point2.y) {
		point2 = Generate_Point()
	}
	point3 := Generate_Point()
	for (point3.x==point1.x && point3.x==point2.x) || (point3.y==point1.y && point3.y==point2.y) {
		point3 = Generate_Point()
	}
	path = append(path , point1)
	path = append(path, point2)
	path = append(path, point3)
	if sides == 3 {
		for i:=0;i<sides;i++ {
			fmt.Printf("  - (%v, %v)\n", path[i].x,path[i].y)
		}
		return path
	}
	for i:=3;i<sides;i++ {
		valid_point := false
		point := Generate_Point()
		for !valid_point {
			for j:=0;j<i-2;j++{
			   if Intersect(path[j],path[j+1], path[i-1], point) {
				   break
			   } 
			   if i==sides-1 {
				   if Intersect(path[j+1],path[j+2], point, path[0]) {
					   break
				   }					
			   }
			   valid_point = true
			}
			if !valid_point {
				point = Generate_Point()
			}
		}
		path = append(path, point)
   }
   for i:=0;i<sides;i++ {
		fmt.Printf("  - (%v, %v)\n", path[i].x,path[i].y)
   }
   return path
}

func Get_Perimeter(path []Point, sides int) {
	fmt.Println("- Figure's Perimeter")
	fmt.Printf("  - ")
	var p float64
	var side float64
	for i:=0;i<sides-2;i++{
		side = Distance(path[i],path[i+1])
		p += side
		fmt.Printf("%v + ",side)
	}
	side = Distance(path[sides-1],path[0])
	p += side
	fmt.Printf("%v = %v\n", side, p)
}

func Generate_Figure(sides int){
	fmt.Printf("- Generating a [%d] sides figure\n", sides)
	fmt.Println("- Figure's vertices")

	var path Path

	path = Generate_Path(path, sides)
	Get_Perimeter(path, sides)
}

//!-path

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage: go run <numer of sides>")
		os.Exit(1)
	}

	sides, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println(err)
		os.Exit(2)
	}

	if sides < 3 {
		fmt.Println("Sides: number of sides must be equal or greater than 3")
		os.Exit(1)
	}

	Generate_Figure(sides)
}

