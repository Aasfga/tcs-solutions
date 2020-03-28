package main

import (
	"fmt"
	. "general"
	"math/rand"
	"os"
	"sync"
	"time"
)


type Channel struct {
	traitors int
	generals []IGeneral
}

func (channel Channel) SendTo(general *General, receiverId int, serializedQuestion []byte) bool {
	ms := string(serializedQuestion)
	if int(ms[0]) - 48 == general.GetId() {
		fmt.Println("KIERWA DEBILU", ms)
	}
	if receiverId < channel.traitors {
		return (rand.Int() % 2) == 1
	}
	return channel.generals[receiverId].Answer(general.GetId(), serializedQuestion)
}
func main() {

	tests := 1
	for k := 0; k < tests; k++ {
		fmt.Println("--------------test: ", k)

		seed := time.Now().UTC().UnixNano()
		rand.Seed(seed)

//		n,traitors:=4,1
		n := 20//rand.Int()%10 + 1
		traitors := 6//rand.Int() % ((n-1)/3 + 1)
		//traitors := 0
		//fmt.Scanf("%d", &n)
		//fmt.Scanf("%d", &t)
		fmt.Printf("Total: %d\nTraitors: %d\n", n, traitors)

		opinions := make([]bool, n)
		generals := make([]IGeneral, n)
		channel := Channel{traitors, generals}
		for i := 0; i < n; i++ {
			opinions[i] = (rand.Int() % 2) == 1
			generals[i] = NewGeneral(i, opinions[i], n, traitors, channel)
		}
		fmt.Println("Original opinions: ", opinions)


		multithreading := false

		if multithreading {
			var wg sync.WaitGroup
			wg.Add(n)
			for index := 0; index < n; index++ {
				go func(index int) {
					defer wg.Done()
					decision := generals[index].MakeDecision()
					for j := traitors; j < n; j++ {
						if (opinions[j] != decision [j]) {
							fmt.Println("ANS\n")
							os.Exit(0)
						}
					}
				}(index)
			}
			wg.Wait()

		} else {
			for i := traitors; i < n; i++ {
				decision := generals[i].MakeDecision()
				fmt.Println(i, ": ", decision)
				for j := traitors; j < n; j++ {
					if (opinions[j] != decision [j]) {
						fmt.Println("ANS\n")
						os.Exit(0)
					}
				}
			}
		}
	}
}
