package main

import (
	"fmt"
)

type Node struct {
	value string
	next  *Node
}

type SList struct {
	head *Node
}

func sAddHead(l *SList, value string) {
	l.head = &Node{value: value, next: l.head}
}

func findSNode(l *SList, value string) *Node {
	cur := l.head
	for cur != nil && cur.value != value {
		cur = cur.next
	}
	return cur
}

func sAddBefore(l *SList, target, value string) bool {
	if l.head == nil {
		return false
	}
	if l.head.value == target {
		sAddHead(l, value)
		return true
	}
	cur := l.head
	for cur.next != nil && cur.next.value != target {
		cur = cur.next
	}
	if cur.next == nil {
		return false
	}
	cur.next = &Node{value: value, next: cur.next}
	return true
}

func sAddAfter(l *SList, target, value string) bool {
	found := findSNode(l, target)
	if found == nil {
		return false
	}
	found.next = &Node{value: value, next: found.next}
	return true
}

func sAddTail(l *SList, value string) {
	n := &Node{value: value}
	if l.head == nil {
		l.head = n
		return
	}
	cur := l.head
	for cur.next != nil {
		cur = cur.next
	}
	cur.next = n
}

func sDelHead(l *SList) (string, bool) {
	if l.head == nil {
		return "", false
	}
	value := l.head.value
	l.head = l.head.next
	return value, true
}

func sDelBefore(l *SList, target string) (string, bool) {

	if l.head == nil || l.head.next == nil || l.head.value == target {
		return "", false
	}
	if l.head.next.value == target {
		return sDelHead(l)
	}
	cur := l.head
	for cur.next.next != nil && cur.next.next.value != target {
		cur = cur.next
	}
	if cur.next.next == nil {
		return "", false
	}
	victim := cur.next
	cur.next = victim.next
	return victim.value, true
}

func sDelAfter(l *SList, target string) (string, bool) {
	found := findSNode(l, target)
	if found == nil || found.next == nil {
		return "", false
	}
	victim := found.next
	found.next = victim.next
	return victim.value, true
}

func sDelTail(l *SList) (string, bool) {
	if l.head == nil {
		return "", false
	}
	if l.head.next == nil {
		return sDelHead(l)
	}
	cur := l.head
	for cur.next.next != nil {
		cur = cur.next
	}
	value := cur.next.value
	cur.next = nil
	return value, true
}

func sDelValue(l *SList, value string) bool {
	var prev *Node
	cur := l.head
	for cur != nil && cur.value != value {
		prev = cur
		cur = cur.next
	}
	if cur == nil {
		return false
	}
	if prev == nil {
		l.head = cur.next
	} else {
		prev.next = cur.next
	}
	return true
}

func sFind(l *SList, value string) int {
	index := 0
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.value == value {
			return index
		}
		index++
	}
	return -1
}

func sGet(l *SList, index int) (string, bool) {
	if index < 0 {
		return "", false
	}
	cur := l.head
	for cur != nil && index > 0 {
		cur = cur.next
		index--
	}
	if cur == nil {
		return "", false
	}
	return cur.value, true
}

func sGetHead(l *SList) (string, bool) {
	if l.head == nil {
		return "", false
	}
	return l.head.value, true
}

func sGetTail(l *SList) (string, bool) {
	if l.head == nil {
		return "", false
	}
	cur := l.head
	for cur.next != nil {
		cur = cur.next
	}
	return cur.value, true
}

func sValues(l *SList) []string {
	values := []string{}
	for cur := l.head; cur != nil; cur = cur.next {
		values = append(values, cur.value)
	}
	return values
}

func printSList(l *SList) {
	for cur := l.head; cur != nil; cur = cur.next {
		fmt.Print(cur.value, " -> ")
	}
	fmt.Println("NULL")
}

type DNode struct {
	value string
	prev  *DNode
	next  *DNode
}

type DList struct {
	head *DNode
	tail *DNode
}

func unlinkNode(l *DList, node *DNode) {
	if node.prev != nil {
		node.prev.next = node.next
	} else {
		l.head = node.next
	}
	if node.next != nil {
		node.next.prev = node.prev
	} else {
		l.tail = node.prev
	}
}

func dAddHead(l *DList, value string) {
	node := &DNode{value: value, next: l.head}
	if l.head != nil {
		l.head.prev = node
	} else {
		l.tail = node
	}
	l.head = node
}

func dAddTail(l *DList, value string) {
	node := &DNode{value: value, prev: l.tail}
	if l.tail != nil {
		l.tail.next = node
	} else {
		l.head = node
	}
	l.tail = node
}

func findDNode(l *DList, value string) *DNode {
	cur := l.head
	for cur != nil && cur.value != value {
		cur = cur.next
	}
	return cur
}

func dAddBefore(l *DList, target, value string) bool {
	found := findDNode(l, target)
	if found == nil {
		return false
	}
	if found.prev == nil {
		dAddHead(l, value)
		return true
	}
	node := &DNode{value: value, prev: found.prev, next: found}
	found.prev.next = node
	found.prev = node
	return true
}

func dAddAfter(l *DList, target, value string) bool {
	found := findDNode(l, target)
	if found == nil {
		return false
	}
	if found.next == nil {
		dAddTail(l, value)
		return true
	}
	node := &DNode{value: value, prev: found, next: found.next}
	found.next.prev = node
	found.next = node
	return true
}
func dDelHead(l *DList) (string, bool) {
	if l.head == nil {
		return "", false
	}
	value := l.head.value
	unlinkNode(l, l.head)
	return value, true
}

func dDelBefore(l *DList, target string) (string, bool) {
	found := findDNode(l, target)
	if found == nil || found.prev == nil {
		return "", false
	}
	victim := found.prev
	unlinkNode(l, victim)
	return victim.value, true
}

func dDelAfter(l *DList, target string) (string, bool) {
	found := findDNode(l, target)
	if found == nil || found.next == nil {
		return "", false
	}
	victim := found.next
	unlinkNode(l, victim)
	return victim.value, true
}

func dDelTail(l *DList) (string, bool) {
	if l.tail == nil {
		return "", false
	}
	value := l.tail.value
	unlinkNode(l, l.tail)
	return value, true
}

func dDelValue(l *DList, value string) bool {
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.value == value {
			unlinkNode(l, cur)
			return true
		}
	}
	return false
}

func dFind(l *DList, value string) int {
	index := 0
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.value == value {
			return index
		}
		index++
	}
	return -1
}

func dGet(l *DList, index int) (string, bool) {
	if index < 0 {
		return "", false
	}
	cur := l.head
	for cur != nil && index > 0 {
		cur = cur.next
		index--
	}
	if cur == nil {
		return "", false
	}
	return cur.value, true
}

func dGetHead(l *DList) (string, bool) {
	if l.head == nil {
		return "", false
	}
	return l.head.value, true
}

func dGetTail(l *DList) (string, bool) {
	if l.tail == nil {
		return "", false
	}
	return l.tail.value, true
}

func dValues(l *DList) []string {
	values := []string{}
	for cur := l.head; cur != nil; cur = cur.next {
		values = append(values, cur.value)
	}
	return values
}

func printDList(l *DList) {
	line := "NULL"
	for cur := l.head; cur != nil; cur = cur.next {
		line += " <-> " + cur.value
	}
	fmt.Println(line + " <-> NULL")
}
