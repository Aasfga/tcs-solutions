package orientation

import (
	"encoding/json"
)

func max(a, b uint64) uint64 {
	if a > b {
		return a
	}
	return b
}

func sendMessage(msg uint64, output chan<- []byte) {
	array := make([]uint64, 1)
	array[0] = msg
	bytes, _ := json.Marshal(array)
	output <- bytes
}

func receiveMessage(input <-chan []byte) uint64 {
	bytes := <-input
	var arr []uint64
	_ = json.Unmarshal(bytes, &arr)
	return arr[0]
}

func SelectLeader(id uint64, input <-chan []byte, output chan<- []byte) uint64 {
	alive := true
	var q, p, r uint64 = 0, 1, id

	for ; ; {
		if alive {
			sendMessage(r, output)
		}

		p = receiveMessage(input)
		sendMessage(p, output)

		q = receiveMessage(input)
		if !alive {
			sendMessage(q, output)
			if p == q {
				break
			} else {
				continue
			}
		}
		if max(q, r) < p {
			r = p
		} else if max(q, r) > p {
			alive = false
		} else if max(q, r) == p {
			break
		}

	}

	return p
}
