package main

import (
	"fmt"
	"strings"
)

type TreeNode struct {
	value string
	left  *TreeNode
	right *TreeNode
}

type Tree struct {
	root *TreeNode
	size int
}

func getNode(root *TreeNode, index int) *TreeNode {
	if index == 0 {
		return root
	}
	parent := getNode(root, (index-1)/2)
	if index%2 == 1 {
		return parent.left
	}
	return parent.right
}

func treeInsert(t *Tree, value string) {
	n := &TreeNode{value: value}
	index := t.size
	t.size++
	if index == 0 {
		t.root = n
		return
	}
	parent := getNode(t.root, (index-1)/2)
	if index%2 == 1 {
		parent.left = n
	} else {
		parent.right = n
	}
}

func findNode(n *TreeNode, value string) bool {
	if n == nil {
		return false
	}
	if n.value == value {
		return true
	}
	return findNode(n.left, value) || findNode(n.right, value)
}

func treeFind(t *Tree, value string) bool {
	return findNode(t.root, value)
}

func checkComplete(n *TreeNode, index, total int) bool {
	if n == nil {
		return true
	}
	if index >= total {
		return false
	}
	return checkComplete(n.left, 2*index+1, total) && checkComplete(n.right, 2*index+2, total)
}

func treeIsComplete(t *Tree) bool {
	return checkComplete(t.root, 0, t.size)
}

func treeValues(t *Tree) []string {
	values := []string{}
	for i := 1; i <= t.size; i++ {
		values = append(values, getNode(t.root, i).value)
	}
	return values
}

func printSideways(n *TreeNode, depth int) {
	if n == nil {
		return
	}
	printSideways(n.right, depth+1)
	fmt.Println(strings.Repeat("    ", depth) + n.value)
	printSideways(n.left, depth+1)
}

func printTree(t *Tree) {
	if t.root == nil {
		fmt.Println("(Пустое дерево)")
		return
	}
	fmt.Println("Level order: " + strings.Join(treeValues(t), " "))
	fmt.Println("Дерево (повернутое на 90 градусов, корень слева, правый дочерний элемент сверху):")
	printSideways(t.root, 0)
}
