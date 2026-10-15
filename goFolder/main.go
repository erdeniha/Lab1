package main

import (
	"fmt"
	"os"
)

func main() {
	file := ""
	query := ""
	args := os.Args[1:]
	for i := 0; i+1 < len(args); i += 2 {
		if args[i] == "--file" {
			file = args[i+1]
		} else if args[i] == "--query" {
			query = args[i+1]
		}
	}

	if file == "" || query == "" {
		fmt.Println("Usage: ./dbms --file <file> --query '<COMMAND args>'")
		os.Exit(1)
	}

	if err := runQuery(file, query); err != nil {
		fmt.Println("-> ERROR:", err)
		os.Exit(1)
	}
}
