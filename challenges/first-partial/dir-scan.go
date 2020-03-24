package main

import (
	"fmt"
	"os"
	"path/filepath"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {
	var scanInfo [5]int

	err := filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			fmt.Printf("%q: %v\n", path, err)
			return err
		}
		switch mode := info.Mode(); {
		case mode&os.ModeDir != 0:
			scanInfo[0] += 1
			break
		case mode&os.ModeSymlink != 0:
			scanInfo[1] += 1
			break
		case mode&os.ModeDevice != 0:
			scanInfo[2] += 1
			break
		case mode&os.ModeSocket != 0:
			scanInfo[3] += 1
			break
		default:
			scanInfo[4] += 1
		}
		
		return nil
	})
	if err == nil {
                fmt.Printf("Directory Scanner Tool\n")
		fmt.Printf("+-------------------------+------+\n")
		fmt.Printf("| Path                    | %s |\n", dir)
		fmt.Printf("+-------------------------+------+\n")
		fmt.Printf("| Directories             | %d   |\n", scanInfo[0])
		fmt.Printf("| Symbolic Links          | %d   |\n", scanInfo[1])
		fmt.Printf("| Devices                 | %d   |\n", scanInfo[2])
		fmt.Printf("| Sockets                 | %d   |\n", scanInfo[3])
		fmt.Printf("| Others                  | %d   |\n", scanInfo[4])
		fmt.Printf("+-------------------------+------+\n")
	}
	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}
	var i=1
	if len(os.Args) > 2 {
		i = 2
	} 
	scanDir(os.Args[i])
}

