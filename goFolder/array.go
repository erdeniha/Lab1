package main

import (
	"fmt"
	"strings"
)

type Array struct {
	data []string
	size int
}

func newArray() *Array {
	return &Array{data: make([]string, 4)}
}

func arrayPush(a *Array, value string) {
	if a.size == len(a.data) {
		bigger := make([]string, len(a.data)*2)
		copy(bigger, a.data)
		a.data = bigger
	}
	a.data[a.size] = value
	a.size++
}

func arrayInsert(a *Array, index int, value string) bool {
	if index < 0 || index > a.size {
		return false
	}
	if a.size == len(a.data) {
		bigger := make([]string, len(a.data)*2)
		copy(bigger, a.data)
		a.data = bigger
	}
	for i := a.size; i > index; i-- {
		a.data[i] = a.data[i-1]
	}
	a.data[index] = value
	a.size++
	return true
}

func arrayGet(a *Array, index int) (string, bool) {
	if index < 0 || index >= a.size {
		return "", false
	}
	return a.data[index], true
}

func arrayDel(a *Array, index int) (string, bool) {
	if index < 0 || index >= a.size {
		return "", false
	}
	value := a.data[index]
	for i := index; i < a.size-1; i++ {
		a.data[i] = a.data[i+1]
	}
	a.size--
	return value, true
}

func arraySet(a *Array, index int, value string) bool {
	if index < 0 || index >= a.size {
		return false
	}
	a.data[index] = value
	return true
}

func arrayValues(a *Array) []string {
	return append([]string{}, a.data[:a.size]...)
}

func printArray(a *Array) {
	fmt.Printf("[%s] (size=%d)\n", strings.Join(arrayValues(a), ", "), a.size)
}
