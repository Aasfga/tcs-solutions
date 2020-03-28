package general

import (
	"encoding/json"
)

type tree struct {
	children map[int]tree
	value    *bool
}

type IChannel interface {
	SendTo(general *General, receiverId int, serializedQuestion []byte) bool
}

type IGeneral interface {
	MakeDecision() []bool
	Answer(senderId int, serializedQuestion []byte) bool
	GetId() int
}

type General struct {
	id               int
	numberOfGenerals int
	numberOfTraitors int
	channel          IChannel
	database         tree
}

func NewGeneral(id int, initialOpinion bool, numberOfGenerals int, numberOfTraitors int, channel IChannel) IGeneral {
	general := new(General)
	general.id = id
	general.numberOfGenerals = numberOfGenerals
	general.numberOfTraitors = numberOfTraitors
	general.channel = channel
	general.database = createTree(initialOpinion)
	return general
}

func (general *General) GetId() int {
	return general.id
}

func (general *General) MakeDecision() []bool {
	generals := otherGenerals(general.numberOfGenerals-1, general.id)
	general.fillDatabase(general.numberOfTraitors+1, generals, []int{})
	general.aggregateAnswers()
	decision := general.retrieveAnswer()

	return decision
}

func (general *General) fillDatabase(depth int, generals []int, path []int) {
	if depth == 0 {
		return
	}
	//subsetGenerals := make([]int, len(generals))
	//copy(subsetGenerals, generals)
	//general.fillDatabase(depth-1, append(subsetGenerals[:i], subsetGenerals[i+1:]...), append(path, g))
	for i, g := range generals {
		general.checkDatabase(append(path, g))
		x, y := generals[0], generals[i]
		generals[0], generals[i] = y, x
		general.fillDatabase(depth-1, generals[1:], append(path, g))
		generals[0], generals[i] = x, y
	}
}

func (general *General) aggregateAnswers() {
	for _, child := range general.database.children {
		child.makeCorrections()
	}
}

func (t *tree) makeCorrections() {
	trues := 0
	falses := 0

	if *t.value {
		trues++
	} else {
		falses++
	}

	for _, child := range t.children {
		child.makeCorrections()
		if *child.value {
			trues++
		} else {
			falses++
		}
	}
	*t.value = falses < trues
}

func (general *General) Answer(senderId int, question []byte) bool {
	return general.checkDatabase(deserialize(question))
}

func (t *tree) getValue(key []int, path []int, general *General) bool {
	if len(key) == 0 {
		return *t.value
	}

	friend := key[0]
	child, ok := t.children[friend]

	if !ok {
		value := general.ask(path, friend)
		child = createTree(value)
		t.children[friend] = child
	}
	return child.getValue(key[1:], append(path, friend), general)
}

func createTree(value bool) tree {
	return tree{make(map[int]tree), &value}
}

func deserialize(question []byte) []int {
	var arr []int
	_ = json.Unmarshal(question, &arr)
	return arr
}

func serialize(question []int) []byte {
	bytes, _ := json.Marshal(question)
	return bytes
}

func (general *General) ask(question []int, friend int) bool {
	return general.channel.SendTo(general, friend, serialize(question))
}

func (general *General) checkDatabase(question []int) bool {
	return general.database.getValue(question, []int{}, general)
}

func otherGenerals(size, id int) []int {
	array := make([]int, size)
	x := 0
	for i := range array {
		if i == id {
			x++
		}
		array[i] = i + x
	}
	return array
}

func (general *General) retrieveAnswer() []bool {
	array := make([]bool, general.numberOfGenerals)
	array[general.id] = *general.database.value
	for k, v := range general.database.children {
		array[k] = *v.value
	}
	return array
}

func (t *tree) getSize() int {
	sum := 0
	for _, v := range t.children {
		sum += v.getSize()
	}
	return sum + 1
}
