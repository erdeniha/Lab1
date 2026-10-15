package main

import (
	"fmt"
	"strings"
)

type Stack struct {
	head *Node
}

func stackPush(s *Stack, value string) {
	s.head = &Node{value: value, next: s.head}
}

func stackPop(s *Stack) (string, bool) {
	if s.head == nil {
		return "", false
	}
	value := s.head.value
	s.head = s.head.next
	return value, true
}

func stackPeek(s *Stack) (string, bool) {
	if s.head == nil {
		return "", false
	}
	return s.head.value, true
}

func stackValues(s *Stack) []string {
	values := []string{}
	for cur := s.head; cur != nil; cur = cur.next {
		values = append(values, cur.value)
	}
	return values
}

func printStack(s *Stack) {
	if s.head == nil {
		fmt.Println("(empty stack)")
		return
	}
	for cur := s.head; cur != nil; cur = cur.next {
		line := "| " + cur.value + " |"
		if cur == s.head {
			line += " <- top"
		}
		fmt.Println(line)
	}
}

type Queue struct {
	head *Node
	tail *Node
}

func queuePush(q *Queue, value string) {
	n := &Node{value: value}
	if q.tail == nil {
		q.head = n
	} else {
		q.tail.next = n
	}
	q.tail = n
}

func queuePop(q *Queue) (string, bool) {
	if q.head == nil {
		return "", false
	}
	value := q.head.value
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	return value, true
}

func queuePeek(q *Queue) (string, bool) {
	if q.head == nil {
		return "", false
	}
	return q.head.value, true
}

func queueValues(q *Queue) []string {
	values := []string{}
	for cur := q.head; cur != nil; cur = cur.next {
		values = append(values, cur.value)
	}
	return values
}

func printQueue(q *Queue) {
	if q.head == nil {
		fmt.Println("(empty queue)")
		return
	}
	fmt.Println("front -> " + strings.Join(queueValues(q), " ") + " <- back")
}
