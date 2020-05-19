/***********************************************************************************
Construct a pipeline that connects an arbitrary number of goroutines with channels. 
What is the maximum number of pipeline stages you can create without running out of 
memory? How long does a value take to transit the entire pipeline?
***********************************************************************************/

package main

import (
	"fmt"
	"os"
	"time"
	"strconv"
)

func passPipe(prev <- chan string, next chan <- string){
	x := <- prev
	next <- x
}

func main() {
	
	if len(os.Args) < 2{
		fmt.Println("Usage: go run pipelines.go <number of pipes>")
		return
	}

	n, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println(err)
		return
	}

	file, err := os.Create("pipeline_report.txt")
	if err != nil {
		fmt.Println(err)
		return
	}

	intro := "This is the report for the exercise 9.4\n----------------------------------------\nMax number of n before losing patience: 900000\nRuntime results->\n"
	file.WriteString(intro)

	line := make([]chan string,n)
	for i:=0; i < n; i++ {
		line[i] = make(chan string)
	}

	head := line[0]
	tail := line[n-1]

	for i:=0; i < n-1; i++ {
		go passPipe(line[i], line[i+1])
	}

	start := time.Now()

	head <- "yeet"
	<- tail

	since := time.Since(start).Seconds()

	fmt.Println("Time transcurred in seconds: ", since)
	fmt.Println("Number of pipes: ", n)
	file.WriteString("Number of pipes: "+strconv.Itoa(n)+"\nTime transcurred in seconds: "+fmt.Sprint(since)+"\n")
	fmt.Println("Report generated at file: pipeline_report.txt")
}
