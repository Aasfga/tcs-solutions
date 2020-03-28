package main

import (
	"./king"
	"fmt"
	"os"
	"runtime"
	"sync"
)

func main() {
	runtime.GOMAXPROCS(1)
	var N int
	fmt.Scanf("%d\n", &N)
	var messageSizeLimit uint64
	fmt.Scanf("%d\n", &messageSizeLimit)
	var messageCountLimit uint64
	fmt.Scanf("%d\n", &messageCountLimit)
	var wg sync.WaitGroup
	writeChannel := make(chan uint64)

	runner := func(value uint64, input <-chan []byte, output chan<- []byte) {
		defer wg.Done()
		writeChannel <- king.SelectLeader(value, input, output)
		close(output)
	}

	type MessageInfo struct {
		MessageSize  uint64
		MessageCount uint64
	}

	dispatchChannel := make(chan []byte)
	messageSizeChannel := make(chan MessageInfo)

	wg.Add(1)
	go func(dispatchChannel chan []byte) {
		defer wg.Done()
		var messageSize, messageCount uint64 = 0, 0
		for message := range dispatchChannel {
			messageCount++
			messageSize += uint64(len(message))
		}
		messageSizeChannel <- MessageInfo{messageSize, messageCount}
	}(dispatchChannel)

	nodeDispatcher := func(output chan []byte, input chan []byte, dispatchChannel chan []byte) {
		defer wg.Done()
		for message := range output {
			dispatchChannel <- message
			input <- message
		}
		close(input)
	}

	firstInput1, firstInput2 := make(chan []byte), make(chan []byte)
	prevOutput := firstInput2
	for it := 0; it < N; it++ {
		var input chan []byte = prevOutput
		var output1, output2 chan []byte
		if it == N-1 {
			output1, output2 = firstInput1, firstInput2
		} else {
			output1, output2 = make(chan []byte), make(chan []byte)
		}

		var value uint64
		fmt.Scanf("%d\n", &value)

		wg.Add(1)
		go nodeDispatcher(output1, output2, dispatchChannel)
		wg.Add(1)
		go runner(value, input, output1)

		prevOutput = output2
	}
	for it := 0; it < N; it++ {
		leader := <-writeChannel
		fmt.Println(leader)
	}

	close(dispatchChannel)

	messageInfo := <-messageSizeChannel
	close(messageSizeChannel)

	fmt.Fprintf(os.Stderr, "Data sent: %d byte(s)", messageInfo.MessageSize)
	fmt.Fprintln(os.Stderr)
	fmt.Printf("Data limit exceeded: %t", messageInfo.MessageSize > messageSizeLimit)
	fmt.Println()
	fmt.Fprintf(os.Stderr, "# of messages sent: %d", messageInfo.MessageCount)
	fmt.Fprintln(os.Stderr)
	fmt.Printf("Limit of messages exceeded: %t", messageInfo.MessageCount > messageCountLimit)
	fmt.Println()

	wg.Wait()
	for runtime.NumGoroutine() > 1 {
	}
}
