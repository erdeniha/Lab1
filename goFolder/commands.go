package main

import (
	"errors"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func parseIndex(s string) (int, bool) {
	if s == "" || len(s) > 9 {
		return 0, false
	}
	for _, c := range s {
		if c < '0' || c > '9' {
			return 0, false
		}
	}
	index, _ := strconv.Atoi(s)
	return index, true
}

func boolText(b bool) string {
	if b {
		return "TRUE"
	}
	return "FALSE"
}

func argCount(cmd string) int {
	switch cmd {
	case "PRINT", "MCREATE", "MLEN", "SPOP", "SGET", "QPOP", "QGET", "TISCOMPLETE":
		return 1
	case "MPUSH", "MGET", "MDEL", "SPUSH", "QPUSH", "TINSERT", "TGET":
		return 2
	case "MINSERT", "MSET":
		return 3
	}
	if cmd[0] == 'F' || cmd[0] == 'L' {
		switch cmd[1:] {
		case "PUSHBEFORE", "PUSHAFTER":
			return 3
		case "PUSHHEAD", "PUSHTAIL", "DELVAL", "FIND", "GET", "DELBEFORE", "DELAFTER":
			return 2
		case "DELHEAD", "DELTAIL", "GETHEAD", "GETTAIL":
			return 1
		}
	}
	return -1
}

func readLines(file string) []string {
	lines := []string{}
	data, err := os.ReadFile(file)
	if err != nil {
		return lines
	}
	for _, line := range strings.Split(string(data), "\n") {
		if strings.TrimSpace(line) != "" {
			lines = append(lines, line)
		}
	}
	return lines
}

func writeLines(file string, lines []string) error {
	return os.WriteFile(file, []byte(strings.Join(lines, "\n")+"\n"), 0644)
}

func findLine(lines []string, name string) int {
	for i, line := range lines {
		fields := strings.Fields(line)
		if len(fields) >= 2 && fields[1] == name {
			return i
		}
	}
	return -1
}

func doArray(cmd string, t, values []string) (string, []string, bool, error) {
	arr := newArray()
	for _, v := range values {
		arrayPush(arr, v)
	}

	result := ""
	changed := false

	switch cmd {
	case "MCREATE":
		result, changed = "OK", true
	case "MPUSH":
		arrayPush(arr, t[2])
		result, changed = t[2], true
	case "MLEN":
		result = strconv.Itoa(arr.size)
	default:
		index, ok := parseIndex(t[2])
		if !ok {
			return "", nil, false, errors.New("invalid index")
		}
		switch cmd {
		case "MINSERT":
			ok = arrayInsert(arr, index, t[3])
			result, changed = t[3], true
		case "MGET":
			result, ok = arrayGet(arr, index)
		case "MDEL":
			result, ok = arrayDel(arr, index)
			changed = true
		case "MSET":
			ok = arraySet(arr, index, t[3])
			result, changed = t[3], true
		}
		if !ok {
			return "", nil, false, errors.New("index out of range")
		}
	}
	return result, arrayValues(arr), changed, nil
}

func doSList(cmd string, t, values []string) (string, []string, bool, error) {
	list := &SList{}
	for i := len(values) - 1; i >= 0; i-- {
		sAddHead(list, values[i])
	}

	result := ""
	changed := false
	ok := true
	op := cmd[1:]

	if (op == "PUSHBEFORE" || op == "PUSHAFTER" || op == "DELBEFORE" || op == "DELAFTER") && sFind(list, t[2]) < 0 {
		return "", nil, false, errors.New("element '" + t[2] + "' not found")
	}

	switch op {
	case "PUSHHEAD":
		sAddHead(list, t[2])
		result, changed = t[2], true
	case "PUSHTAIL":
		sAddTail(list, t[2])
		result, changed = t[2], true
	case "PUSHBEFORE":
		ok = sAddBefore(list, t[2], t[3])
		result, changed = t[3], true
	case "PUSHAFTER":
		ok = sAddAfter(list, t[2], t[3])
		result, changed = t[3], true
	case "DELHEAD":
		result, ok = sDelHead(list)
		changed = true
	case "DELTAIL":
		result, ok = sDelTail(list)
		changed = true
	case "DELBEFORE":
		result, ok = sDelBefore(list, t[2])
		changed = true
	case "DELAFTER":
		result, ok = sDelAfter(list, t[2])
		changed = true
	case "DELVAL":
		changed = sDelValue(list, t[2])
		result = boolText(changed)
	case "FIND":
		position := sFind(list, t[2])
		if position < 0 {
			result = "NOT_FOUND"
		} else {
			result = strconv.Itoa(position)
		}
	case "GET":
		index, good := parseIndex(t[2])
		if !good {
			return "", nil, false, errors.New("invalid index")
		}
		result, ok = sGet(list, index)
		if !ok {
			return "", nil, false, errors.New("index out of range")
		}
	case "GETHEAD":
		result, ok = sGetHead(list)
	case "GETTAIL":
		result, ok = sGetTail(list)
	}
	if !ok {
		return "", nil, false, errors.New("no such element")
	}
	return result, sValues(list), changed, nil
}

func doDList(cmd string, t, values []string) (string, []string, bool, error) {
	list := &DList{}
	for _, v := range values {
		dAddTail(list, v)
	}

	result := ""
	changed := false
	ok := true
	op := cmd[1:]

	if (op == "PUSHBEFORE" || op == "PUSHAFTER" || op == "DELBEFORE" || op == "DELAFTER") && dFind(list, t[2]) < 0 {
		return "", nil, false, errors.New("element '" + t[2] + "' not found")
	}

	switch op {
	case "PUSHHEAD":
		dAddHead(list, t[2])
		result, changed = t[2], true
	case "PUSHTAIL":
		dAddTail(list, t[2])
		result, changed = t[2], true
	case "PUSHBEFORE":
		ok = dAddBefore(list, t[2], t[3])
		result, changed = t[3], true
	case "PUSHAFTER":
		ok = dAddAfter(list, t[2], t[3])
		result, changed = t[3], true
	case "DELHEAD":
		result, ok = dDelHead(list)
		changed = true
	case "DELTAIL":
		result, ok = dDelTail(list)
		changed = true
	case "DELBEFORE":
		result, ok = dDelBefore(list, t[2])
		changed = true
	case "DELAFTER":
		result, ok = dDelAfter(list, t[2])
		changed = true
	case "DELVAL":
		changed = dDelValue(list, t[2])
		result = boolText(changed)
	case "FIND":
		position := dFind(list, t[2])
		if position < 0 {
			result = "NOT_FOUND"
		} else {
			result = strconv.Itoa(position)
		}
	case "GET":
		index, good := parseIndex(t[2])
		if !good {
			return "", nil, false, errors.New("Неверный индекс")
		}
		result, ok = dGet(list, index)
		if !ok {
			return "", nil, false, errors.New("Индекс вышел за пределы")
		}
	case "GETHEAD":
		result, ok = dGetHead(list)
	case "GETTAIL":
		result, ok = dGetTail(list)
	}
	if !ok {
		return "", nil, false, errors.New("Нет таких элементов")
	}
	return result, dValues(list), changed, nil
}

func doStack(cmd string, t, values []string) (string, []string, bool, error) {
	stack := &Stack{}
	for i := len(values) - 1; i >= 0; i-- {
		stackPush(stack, values[i])
	}

	result := ""
	changed := false
	ok := true

	switch cmd {
	case "SPUSH":
		stackPush(stack, t[2])
		result, changed = t[2], true
	case "SPOP":
		result, ok = stackPop(stack)
		changed = true
	case "SGET":
		result, ok = stackPeek(stack)
	}
	if !ok {
		return "", nil, false, errors.New("Стек пуст")
	}
	return result, stackValues(stack), changed, nil
}

func doQueue(cmd string, t, values []string) (string, []string, bool, error) {
	queue := &Queue{}
	for _, v := range values {
		queuePush(queue, v)
	}

	result := ""
	changed := false
	ok := true

	switch cmd {
	case "QPUSH":
		queuePush(queue, t[2])
		result, changed = t[2], true
	case "QPOP":
		result, ok = queuePop(queue)
		changed = true
	case "QGET":
		result, ok = queuePeek(queue)
	}
	if !ok {
		return "", nil, false, errors.New("Очередь пуста")
	}
	return result, queueValues(queue), changed, nil
}

func doTree(cmd string, t, values []string) (string, []string, bool, error) {
	tree := &Tree{}
	for _, v := range values {
		treeInsert(tree, v)
	}

	result := ""
	changed := false

	switch cmd {
	case "TINSERT":
		treeInsert(tree, t[2])
		result, changed = t[2], true
	case "TGET":
		result = boolText(treeFind(tree, t[2]))
	case "TISCOMPLETE":
		result = boolText(treeIsComplete(tree))
	}
	return result, treeValues(tree), changed, nil
}

func printStructure(kind, name string, values []string) {
	fmt.Printf("[%s] %s:\n", kind, name)

	switch kind {
	case "M":
		arr := newArray()
		for _, v := range values {
			arrayPush(arr, v)
		}
		printArray(arr)
	case "F":
		list := &SList{}
		for i := len(values) - 1; i >= 0; i-- {
			sAddHead(list, values[i])
		}
		printSList(list)
	case "L":
		list := &DList{}
		for _, v := range values {
			dAddTail(list, v)
		}
		printDList(list)
	case "S":
		stack := &Stack{}
		for i := len(values) - 1; i >= 0; i-- {
			stackPush(stack, values[i])
		}
		printStack(stack)
	case "Q":
		queue := &Queue{}
		for _, v := range values {
			queuePush(queue, v)
		}
		printQueue(queue)
	case "T":
		tree := &Tree{}
		for _, v := range values {
			treeInsert(tree, v)
		}
		printTree(tree)
	}
}

func canCreate(cmd string) bool {
	switch cmd {
	case "MCREATE", "MPUSH", "MINSERT", "FPUSHHEAD", "FPUSHTAIL", "LPUSHHEAD", "LPUSHTAIL",
		"SPUSH", "QPUSH", "TINSERT":
		return true
	}
	return false
}

func runQuery(file, query string) error {
	t := strings.Fields(query)
	if len(t) == 0 {
		return errors.New("Пустой запрос")
	}
	t[0] = strings.ToUpper(t[0])
	cmd := t[0]

	count := argCount(cmd)
	if count < 0 {
		return errors.New("Неизвестная команда: " + cmd)
	}
	if len(t) != count+1 {
		return errors.New("Неправильное количество аргументов для " + cmd)
	}

	lines := readLines(file)
	name := t[1]
	lineIndex := findLine(lines, name)

	creates := strings.Contains(cmd, "PUSH") || cmd == "MINSERT" || cmd == "MCREATE" || cmd == "TINSERT"
	if lineIndex < 0 && !creates {
		return errors.New("Структура '" + name + "' не найдена")
	}
	if lineIndex >= 0 && cmd == "MCREATE" {
		return errors.New("Массив уже существует")
	}

	kind := cmd[:1]
	values := []string{}
	if lineIndex >= 0 {
		fields := strings.Fields(lines[lineIndex])
		if cmd == "PRINT" {
			kind = fields[0]
		} else if fields[0] != kind {
			return errors.New("Имя '" + name + "' принадлежит к другому типу структуры")
		}
		values = fields[2:]
	}

	if cmd == "PRINT" {
		printStructure(kind, name, values)
		return nil
	}

	var result string
	var newValues []string
	var changed bool
	var err error

	switch kind {
	case "M":
		result, newValues, changed, err = doArray(cmd, t, values)
	case "F":
		result, newValues, changed, err = doSList(cmd, t, values)
	case "L":
		result, newValues, changed, err = doDList(cmd, t, values)
	case "S":
		result, newValues, changed, err = doStack(cmd, t, values)
	case "Q":
		result, newValues, changed, err = doQueue(cmd, t, values)
	case "T":
		result, newValues, changed, err = doTree(cmd, t, values)
	}
	if err != nil {
		return err
	}
	fmt.Println("->", result)

	if changed {
		line := kind + " " + name
		for _, v := range newValues {
			line += " " + v
		}
		if lineIndex >= 0 {
			lines[lineIndex] = line
		} else {
			lines = append(lines, line)
		}
		return writeLines(file, lines)
	}
	return nil
}
