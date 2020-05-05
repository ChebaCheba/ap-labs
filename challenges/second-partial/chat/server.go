// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"flag"
	"strings"
	"bytes"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel
var isFirstConn bool
var users map[string]net.Conn
var moderator string

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)

func getUsers() string {
	var b bytes.Buffer
	b.WriteString("irc-server[private] > ")
	for user, _ := range users {
		b.WriteString(user + ", ")
	}
	return b.String()
}

func getUserDetails(user string) string {
	if _, ok := users[user]; ok {
		return ("irc-server[private] > username: "+user +", IP: "+users[user].RemoteAddr().String())
	}
	return "irc-server[private] > Invalid username" 
}

func getTime() string {
	var b bytes.Buffer
	b.WriteString("irc-server[private] > Local Time: ")
	b.WriteString(time.Now().Format(time.RFC850))
	return b.String()
}

func kickUser(admin, user string) string {
	if moderator == admin {
		if admin == user {
			fmt.Fprintln(users[admin], "irc-server[private] > Don't be so hard on yourself")
			return ""
		}
		if _, ok := users[user]; ok {
			banned := users[user]
			fmt.Fprintln(users[user], "irc-[private] > You're kicked from this channel")
			fmt.Println("irc-server > "+user+" was kicked")
			delete(users, user)
			banned.Close()
			return "irc-server > "+user+" was kicked from channel"
		}
		fmt.Fprintln(users[admin], "irc-server[private] > Invalid username")
		return ""
	}
	fmt.Fprintln(users[admin], "irc-server[private] > Only the ADMIN can kick a user")
	return ""
}

func sendDirectMessage(from, to string, text []string) {
	if _, ok := users[to]; ok { 
		fmt.Fprintln(users[to], from+"[private] > "+strings.Join(text, " "))
	} else {
		fmt.Fprintln(users[from], "irc-server[private] > Invalid username")
	}
}

func setNewAdmin() string {
	if len(users) == 0 {
		return ""
	}
	for user, _ := range users {
		moderator = user
		break
	}
	fmt.Fprintln(users[moderator], "irc-server[private] > You're the new IRC Server ADMIN")
	return moderator
}

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn, user string) {
	var kickmsg string 
	users[user] = conn
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)
	ch <-  "irc-server > Your user "+user+" is successfully logged"
	messages <- "irc-server > New connected user "+user
	fmt.Println("irc-server > New connected user "+user)
	if isFirstConn {
		isFirstConn = false
		moderator = user
		ch <- "irc-server > Congrats, you were the first user."
		ch <- "irc-server > You're the new IRC Server ADMIN"
		fmt.Println("irc-server > "+user+" was promoted as the channel ADMIN")
	}
	entering <- ch

	input := bufio.NewScanner(conn)
	for input.Scan() {
		text := input.Text()
		text_len := len(text)
		if text_len == 0 {
			messages <- user + " > " + input.Text()
			continue
		}
		if text[0]!='/' {
			messages <- user + " > " + input.Text()
			continue
		}
		cmd := strings.Split(strings.Trim(text, " "), " ")
		if len(cmd) == 1 {
			if cmd[0] == "/users" {
				ch <- getUsers()
				continue
			} else if cmd[0] == "/time" {
				ch <- getTime()
				continue
			}
		} else if len(cmd) == 2 {
			if cmd[0] == "/user" {
				ch <- getUserDetails(cmd[1])
				continue
			} else if cmd[0] == "/kick" {
				kickmsg = kickUser(user,cmd[1])
				if kickmsg != "" {
					messages <- kickmsg
				}
				continue
			}
		} else if len(cmd) > 2 {
			if cmd[0] == "/msg" {
				sendDirectMessage(user, cmd[1], cmd[2:])
				continue
			}
		}
		messages <- user + " > " + input.Text()
	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	if _, ok := users[user]; ok {
		fmt.Fprintln(conn ,"irc-server > You left the channel")
		fmt.Fprintln(conn ,"irc-server > See you, space cowboy")
		messages <- "irc-server > "+user + " left the channel"
		fmt.Println("irc-server > "+user+" left")
		delete(users, user)
		defer conn.Close()
		if user == moderator {
			
			moderator = setNewAdmin()
			if moderator != "" {
				messages <- "irc-server > "+moderator+" was promoted as the channel ADMIN"
				fmt.Println("irc-server > "+moderator+" was promoted as the channel ADMIN")
			}
		}
		if len(users) == 0 {
			isFirstConn = true
		}
	}
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {

	isFirstConn = true
	var host = flag.String("host","localhost","name of host")
	var port = flag.String("port","8000","port number")
	flag.Parse()

	listener, err := net.Listen("tcp", *host+":"+*port)
	if err != nil {
		log.Fatal(err)
	}
	users = make(map[string]net.Conn)
	fmt.Println("irc-server > Simple IRC Server started at "+*host+":"+*port)
	fmt.Println("irc-server > Ready for receiving new clients")
	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		user, _ := bufio.NewReader(conn).ReadString('\n')
		user = strings.TrimSuffix(user, "\n")
		if _, ok := users[user]; ok {
			fmt.Fprintf(conn, "irc-server > Username already exists! Try another one\n")
			conn.Close()
		} else {
			go handleConn(conn, user)
		}
		
	}
}

//!-main

