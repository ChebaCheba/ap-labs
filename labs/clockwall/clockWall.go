package main

import (
	"os"
	"strings"
	"fmt"
	"net"
	"log"
	"io"
)

func handleConnC(host string, wait chan int) {
	conn, err := net.Dial("tcp", host)
	if err != nil {
		log.Fatal(err)
	}
	
	done := make(chan int)
    go func() {
        io.Copy(os.Stdout, conn) // NOTE: ignoring errors
        log.Println("done")
		done <- 2 // signal the main goroutine
		wait <- 1
    }()

    x := 1
    x = <-done // wait for background goroutine to finish
    log.Println("Channel Closed with value: ", x)
    close(done)

	
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("go run clockWall.go <location>=<host>:<port> ...")
		os.Exit(1)
	}

	hPorts := os.Args[1:]
	wait := make(chan int)

	for _, hPort := range hPorts {
		temp := strings.Split(hPort, "=")
		host := temp[1]
		go handleConnC(host, wait)
	}
	<-wait
	close(wait)
}
