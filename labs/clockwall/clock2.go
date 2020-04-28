// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
	"fmt"
)

func handleConn(c net.Conn, tz string) {
	defer c.Close()
	loc, err := time.LoadLocation(tz)
    if err != nil {
        log.Fatal(err)
    }
	for {
		_, err := io.WriteString(c, tz+" : "+time.Now().In(loc).Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	if len(os.Args) < 3 {
		fmt.Println("Error: usage should be:TZ=<timezone> go run clock2.go -port <port>")
		os.Exit(1)
	}
	tz := os.Getenv("TZ")
	port := os.Args[2]
	listener, err := net.Listen("tcp", "localhost:"+port)
	if err != nil {
		log.Fatal(err)
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, tz) // handle connections concurrently
	}
}

