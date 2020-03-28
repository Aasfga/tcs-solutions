package main

import (
	"fmt"
	"net"
	"net/http"
	"net/rpc"
	"os"
)

type Args struct {
	X, Y int
}

type Arithmetic int

func (t *Arithmetic) Sum(args *Args, reply *int) error {
	*reply = args.X + args.Y
	fmt.Println(*reply)
	return nil
}

func main() {
	arithmetic := new(Arithmetic)
	rpc.Register(arithmetic)
	rpc.HandleHTTP()
	l, _ := net.Listen("tcp", os.Args[1])
	http.Serve(l, nil)
}
