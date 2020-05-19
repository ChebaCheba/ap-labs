/*************************************************************************
Write a program with two goroutines that send messages back and forth over
two unbuffered channels in ping-pong fashion. How many communications per 
second can the program sustain?
*************************************************************************/
package main

import (
	"fmt"
	"os"
	"strconv"
	"time"
)

func main() {
	if len(os.Args) < 2{
		fmt.Println("Usage: go run pingpong.go <number of seconds for back and forth>")
	}

	n, err := strconv.Atoi(os.Args[1])

	if err != nil {
		fmt.Println(err)
		return
	}

	file, err := os.Create("pingpong_report.txt")
	if err != nil {
		fmt.Println(err)
		return
	}

	intro := "This is the report for the exercise 9.5\n----------------------------------------\n"
	file.WriteString(intro)
	done := make(chan bool)
	ping := make(chan string)
	pong := make(chan string)
	end := make(chan struct{})

	timer := time.NewTimer(time.Duration(n)*time.Second)
	var count int

	go func() {
		loop :
			for {
				select {
				case <- end :
					break loop 
				case msg := <-pong :
					ping <- msg
				}
			}
		done <- true
	}()

	go func() {
		loop :
			for {
				select {
				case <- end :
					break loop
				case msg := <-ping :
					count++
					pong <- msg
				}
			}
		done <- true
	}()

	ping <- "ping"

	<-timer.C
	close(end)
	select {
	case <-ping:
	case <-pong:
	}

	<- done
	<- done
	timer.Stop()
	fmt.Println("Number of back and forths ",count)
	file.WriteString("Number of seconds of runtime: "+strconv.Itoa(n)+"\n")
	file.WriteString("Total number of back and forths: "+strconv.Itoa(count)+"\n")
	cps := int(count/n)
	file.WriteString("Back and forths per second: "+strconv.Itoa(cps)+"\n")
	fmt.Println("Report generated at: pingpong_report.txt")
}
