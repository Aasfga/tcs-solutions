package orient

import (
	"encoding/json"
	"fmt"
)

func max(a, b uint64) uint64 {
	if a > b {
		return a
	}
	return b
}

type RandomBit interface {
	Read() bool
}

func receiveMessage(input <-chan []byte) uint64 {
	var bytes []byte
	bytes = <-input
	var arr []uint64
	_ = json.Unmarshal(bytes, &arr)
	return arr[0]
}

func clearRandom(bit RandomBit) {
	for i := 0; i < 1000; i++ {
		bit.Read()
	}
}

func generateID(bit RandomBit) uint64 {
	id := uint64(0)
	m := uint64(1)
	for i := 0; i < 64; i++ {
		if bit.Read() {
			id += m
		}
		m *= 2
	}
	return id
}

func Orientation(leftInput <-chan []byte, leftOutput chan<- []byte,
	rightInput <-chan []byte, rightOutput chan<- []byte,
	size uint64, rand RandomBit) bool {
	clearRandom(rand)
	id := generateID(rand)
	alive := true
	var leftID, rightID uint64

	for ; ; {
		if alive {
			broadcastID(leftOutput, rightOutput, id)
			leftID, rightID = receiveIDs(leftInput, rightInput)
		} else {
			leftID, rightID = forwardIDs(leftInput, leftOutput, rightInput, rightOutput)
		}

		//fmt.Printf("Process %d - alive: %t, left: %d, right %d\n", id, alive, leftID, rightID)

		if alive && (leftID > id || rightID > id) {
			//fmt.Printf("Process %d is weaker than %d\n", id, max(leftID, rightID))
			alive = false
		} else if leftID == rightID && leftID >= id {
			//fmt.Printf("Process %d dies\n", id)
			break
		}
	}
	if alive {
		broadcastID(leftOutput, nil, id)
		<-rightInput
		return false
	} else {
		select {
		case message := <-rightInput:
			leftOutput <- message
			return false
		case message := <-leftInput:
			rightOutput <- message
			return true
		}
	}
}

func forwardIDs(leftInput <-chan []byte, leftOutput chan<- []byte,
	rightInput <-chan []byte, rightOutput chan<- []byte) (leftID uint64, rightID uint64) {
	select {
	case message := <-leftInput:
		leftID = convertMessage(message)
		rightOutput <- message
		message = <-rightInput
		rightID = convertMessage(message)
		leftOutput <- message
	case message := <-rightInput:
		rightID = convertMessage(message)
		leftOutput <- message
		message = <-leftInput
		leftID = convertMessage(message)
		rightOutput <- message
	}

	return
}

func receiveIDs(leftOutput <-chan []byte, rightOutput <-chan []byte) (leftID uint64, rightID uint64) {
	select {
	case message := <-leftOutput:
		leftID = convertMessage(message)
		rightID = convertMessage(<-rightOutput)
	case message := <-rightOutput:
		rightID = convertMessage(message)
		leftID = convertMessage(<-leftOutput)
	}
	return
}

func broadcastID(left chan<- []byte, right chan<- []byte, id uint64) {
	bytes, err := json.Marshal([]uint64{id})
	if err != nil {
		fmt.Println(err)
	}
	left <- bytes
	if right != nil {
		right <- bytes
	}
}

func convertMessage(bytes []byte) uint64 {
	var arr []uint64
	err := json.Unmarshal(bytes, &arr)
	if err != nil {
		fmt.Println(err)
	}
	return arr[0]
}
