package main

import (
	"encoding/json"
)

type IServer interface {
	getTime(Sender *SSender) int
	sendTo(Sender *SSender, ReceiverId int, data []byte)
}

func max(x, y int) int {
	if x >= y {
		return x
	}
	return y
}

type SSender struct {
	Id              int
	NumberOfSenders int
	Times           []int
	server          IServer
	data            interface{}
}

func NewSender(id int, numberOfSenders int, server IServer) *SSender {
	Sender := new(SSender)
	Sender.Id = id
	Sender.NumberOfSenders = numberOfSenders
	Sender.server = server
	Sender.Times = make([]int, numberOfSenders)
	for i := range Sender.Times {
		Sender.Times[i] = 0
	}
	return Sender
}

func (sender *SSender) send(receiverId int) {
	sender.updateTime()
	sender.server.sendTo(sender, receiverId, serialize(sender.Times))
}

func operator(first, second []int) bool {
	for i := range first {
		if first[i] > second[i] {
			return false
		}
	}
	for i := range first {
		if first[i] < second[i] {
			return true
		}

	}
	return false
}

func (sender *SSender) updateTime() {
	sender.Times[sender.Id] = sender.server.getTime(sender)
}

func update(sender *SSender, second []int) {
	for i := range sender.Times {
		sender.Times[i] = max(sender.Times[i], second[i])
	}
}

func (sender *SSender) receive(data []byte) bool {
	vector := deserialize(data, sender.NumberOfSenders)
	res := operator(vector, sender.Times)
	update(sender, vector)
	sender.updateTime()
	return res
}

func deserialize(bytes []byte, size int) []int {
	array := make([]int, size)
	json.Unmarshal(bytes, &array)
	return array
}

func serialize(array []int) []byte {
	bytes, _ := json.Marshal(array)
	return bytes
}

