package main

import (
	"fmt"

	drivemanager "github.com/litmus-zhang/petrodata/drive_manager"
)

// export Multiply
func Multiply(a, b int) int {
	return a * b
}

// export PrintMessage
func PrintMessage() {
	fmt.Println("A Go Function")
}

func main() {
	drivemanager.DriverInit()
}
