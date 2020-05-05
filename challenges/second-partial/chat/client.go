// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"flag"
)

//!+
func main() {

	var username = flag.String("user","","name of user")
	var server = flag.String("server","localhost:8000","server to connect to")
	flag.Parse()

	if *username == "" {
		log.Fatal("Plase register a username.\n")
		os.Exit(-1)
	} 

	conn, err := net.Dial("tcp", *server)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("irc-server > Welcome to the Simple IRC Server")
	fmt.Fprintf(conn, *username+"\n")

	done := make(chan struct{})
	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("You left the server")
		done <- struct{}{} // signal the main goroutine
	}()
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}

