package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	count := make(map[string]int)
	for _, word := range strings.Split(s, " ") {
		count[word]++
	}
	return count
}

func main() {
	wc.Test(WordCount)
}
