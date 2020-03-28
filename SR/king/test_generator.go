package main

import (
	"fmt"
	"math/rand"
	"os"
	"sort"
	"strconv"
)

func main() {
	var N uint64
	if len(os.Args) < 2 {
		N = 3000
	} else {
		N2, err := strconv.Atoi(os.Args[1])
		if err != nil {
			os.Exit(-1)
		}
		N = uint64(N2)
	}
	sortAscending := false
	sortDescending := false
	if len(os.Args) > 2 {
		var err error
		sortAscending, err = strconv.ParseBool(os.Args[2])
		if err != nil {
			os.Exit(-2)
		}
		sortDescending = !sortAscending
	}

	//rand := rand.New(rand.NewSource(0))
	values := make(map[uint64](struct{}), N)
	valuesArray := make([]uint64, 0, N)
	fmt.Println(N)
	messageSizeLimit, messageCountLimit := 0, 0
	fmt.Println(messageSizeLimit)
	fmt.Println(messageCountLimit)
	for it := uint64(0); it < N; it++ {
		for {
			//value := rand.Uint64()
			value := rand.Uint64() % 13
			if _, ok := values[value]; !ok {
				values[value] = struct{}{}
				valuesArray = append(valuesArray, value)
				break
			}
		}
	}
	if sortAscending {
		sort.Slice(valuesArray, func(i, j int) bool { return valuesArray[i] < valuesArray[j] })
	}
	if sortDescending {
		sort.Slice(valuesArray, func(i, j int) bool { return valuesArray[i] > valuesArray[j] })
	}
	for _, k := range valuesArray {
		fmt.Printf("%d ", k)
		fmt.Println()
	}
}
