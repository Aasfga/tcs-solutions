package main

import (
	"fmt"
	"net/rpc"
	"os"
)

type Args struct {
	X, Y int
}

func main() {
	var x, y int
	fmt.Scanf("%d %d", &x, &y)
	args := &Args{x, y}
	var reply int
	client, _ := rpc.DialHTTP("tcp", os.Args[1])
	client.Call("Arithmetic.Sum", args, &reply)
}
