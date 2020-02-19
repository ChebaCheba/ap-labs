package main

import (
	"fmt"
	"os"
	"bufio"
)

func longest_substring(word string) int {
	characters := make(map[string]int)
	count := 0
	longest := 0
	for i , char := range word {
		if char == '\n' {
			continue
		}
		if _, ok := characters[string(char)]; !(ok) {
			count += 1
		} else{
			temp := characters[string(char)]
			if (i - count) > temp {
				count += 1
			} else {
				if longest < count {
					longest = count
				}
				count = i - temp
			}
		}
		characters[string(char)] = i
	}
	if longest < count {
		longest = count
	}
	return longest
}

func main() {
	reader := bufio.NewReader(os.Stdin)
	//fmt.Println("Introduce a string")
	word, _ := reader.ReadString('\n')

	var longest = longest_substring(word)
	fmt.Println(longest)
}
