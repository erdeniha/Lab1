#include "commands.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "array.h"
#include "list.h"
#include "queue.h"
#include "stack.h"
#include "completebinarytree.h"


enum Op {
    OP_PRINT,
    OP_CREATE,
    OP_PUSH,        
    OP_INSERT_AT,   
    OP_GET,         
    OP_DEL,         
    OP_SET,
    OP_LEN,
    OP_PUSH_HEAD,
    OP_PUSH_TAIL,
    OP_PUSH_BEFORE,  
    OP_PUSH_AFTER,   
    OP_DEL_HEAD,
    OP_DEL_TAIL,
    OP_DEL_BEFORE,
    OP_DEL_AFTER,
    OP_DEL_VALUE,
    OP_FIND,
    OP_GET_HEAD,
    OP_GET_TAIL,
    OP_POP,
    OP_PEEK,
    OP_IS_COMPLETE
};

struct Command {
    const char* name;
    char type;    
    Op op;
    int args;      
    bool creates;  
};

static const Command commands[] = {
    {"PRINT",'?', OP_PRINT,1, false}, //имя, тип, операция, сколько слов после команды, может ли создавать структуру

    {"MCREATE",'M', OP_CREATE,1, true},
    {"MPUSH",'M', OP_PUSH,2, true},
    {"MINSERT",'M', OP_INSERT_AT,3, true},
    {"MGET",'M', OP_GET,2, false},
    {"MDEL",'M', OP_DEL,2, false},
    {"MSET",'M', OP_SET,3, false},
    {"MLEN",'M', OP_LEN,1, false},

    {"FPUSHHEAD",'F', OP_PUSH_HEAD,2, true},
    {"FPUSHBEFORE",    'F', OP_PUSH_BEFORE,        3, false},
    {"FPUSHAFTER",     'F', OP_PUSH_AFTER,         3, false},
    {"FPUSHTAIL",'F', OP_PUSH_TAIL,2, true},
    {"FDELHEAD",'F', OP_DEL_HEAD,1, false},
    {"FDELBEFORE",     'F', OP_DEL_BEFORE,         2, false},
    {"FDELAFTER",      'F', OP_DEL_AFTER,          2, false},
    {"FDELTAIL",'F', OP_DEL_TAIL,1, false},
    {"FDELVAL",'F', OP_DEL_VALUE,2, false},
    {"FFIND",'F', OP_FIND,2, false},
    {"FGET",'F', OP_GET,2, false},
    {"FGETHEAD",'F', OP_GET_HEAD,1, false},
    {"FGETTAIL",'F', OP_GET_TAIL,1, false},

    {"LPUSHHEAD",'L', OP_PUSH_HEAD,2, true},
    {"LPUSHAFTER",'L', OP_PUSH_AFTER,3, false},
    {"LPUSHBEFORE", 'L', OP_PUSH_BEFORE,3, false},
    {"LPUSHTAIL",'L', OP_PUSH_TAIL,2, true},
    {"LDELHEAD",'L', OP_DEL_HEAD,1, false},
    {"LDELAFTER",'L', OP_DEL_AFTER,2, false},
    {"LDELBEFORE",'L', OP_DEL_BEFORE,2, false},
    {"LDELTAIL",'L', OP_DEL_TAIL,1, false},
    {"LDELVAL",'L', OP_DEL_VALUE,2, false},
    {"LFIND",'L', OP_FIND,2, false},
    {"LGET",'L', OP_GET,2, false},
    {"LGETHEAD",'L', OP_GET_HEAD,1, false},
    {"LGETTAIL",'L', OP_GET_TAIL,1, false},

    {"SPUSH",'S', OP_PUSH,2, true},
    {"SPOP",'S', OP_POP,1, false},
    {"SGET",'S', OP_PEEK,1, false},

    {"QPUSH",'Q', OP_PUSH,2, true},
    {"QPOP",'Q', OP_POP,1, false},
    {"QGET",'Q', OP_PEEK,1, false},

    {"TINSERT",'T', OP_PUSH,2, true},
    {"TGET",'T', OP_FIND,2, false},
    {"TISCOMPLETE",'T', OP_IS_COMPLETE,1, false}
};

static const Command* findCommand(const std::string& name) {
    int count = sizeof(commands) / sizeof(commands[0]);
    for (int i = 0; i < count; i++) {
        if (name == commands[i].name) {
            return &commands[i];
        }
    }
    return nullptr;
}


static std::vector<std::string> split(const std::string& text) {
    std::vector<std::string> parts;
    std::istringstream stream(text);
    std::string word;
    while (stream >> word) {
        parts.push_back(word);
    }
    return parts;
}

static std::string toUpper(std::string s) {
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 'a' + 'A';
        }
    }
    return s;
}

static bool parseIndex(const std::string& s, int& index) {
    if (s.empty() || s.size() > 9) {
        return false;
    }
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    index = std::stoi(s);
    return true;
}

static bool fail(const std::string& message) {
    std::cout << "-> ERROR: " << message << std::endl;
    return false;
}

static std::vector<std::string> readLines(const std::string& file) {
    std::vector<std::string> lines;
    std::ifstream in(file);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    return lines;
}

static bool writeLines(const std::string& file, const std::vector<std::string>& lines) {
    std::ofstream out(file);
    if (!out) {
        return false;
    }
    for (size_t i = 0; i < lines.size(); i++) {
        out << lines[i] << "\n";
    }
    return true;
}

static int findLine(const std::vector<std::string>& lines, const std::string& name) {
    for (size_t i = 0; i < lines.size(); i++) {
        std::vector<std::string> fields = split(lines[i]);
        if (fields.size() >= 2 && fields[1] == name) {
            return (int)i;
        }
    }
    return -1;
}

static std::string doArray(Op op, const std::vector<std::string>& t
    , std::vector<std::string>& values, std::string& result, bool& changed) {

    Array arr;
    initArray(&arr);
    for (size_t i = 0; i < values.size(); i++) {
        arrayPush(&arr, values[i]);
    }

    std::string error = "";
    int index = 0;
    bool ok = true;
    bool hasIndex = (op == OP_INSERT_AT || op == OP_GET || op == OP_DEL || op == OP_SET);

    if (hasIndex && !parseIndex(t[2], index)) {
        error = "Неверный индекс";
    } else {
        switch (op) {
            case OP_CREATE:
                result = "OK";
                changed = true;
                break;
            case OP_PUSH:
                arrayPush(&arr, t[2]);
                result = t[2];
                changed = true;
                break;
            case OP_LEN:
                result = std::to_string(arrayLength(&arr));
                break;
            case OP_INSERT_AT:
                ok = arrayInsert(&arr, index, t[3]);
                result = t[3];
                changed = ok;
                break;
            case OP_GET:
                ok = arrayGet(&arr, index, result);
                break;
            case OP_DEL:
                ok = arrayDel(&arr, index, result);
                changed = ok;
                break;
            case OP_SET:
                ok = arraySet(&arr, index, t[3]);
                result = t[3];
                changed = ok;
                break;
            default:
                break;
        }
        if (!ok) {
            error = "Индекс вне диапазона";
        }
    }

    if (error == "" && changed) {
        values.clear();
        arrayToValues(&arr, values);
    }
    freeArray(&arr);
    return error;
}

static std::string doSList(Op op, const std::vector<std::string>& t,
                           std::vector<std::string>& values, std::string& result, bool& changed) {
    SList list;
    initSList(&list);
    for (int i = (int)values.size() - 1; i >= 0; i--) {
        sAddHead(&list, values[i]);  
    }

    std::string error = "";
    int index = 0;
    bool ok = true;
    bool hasTarget = (op == OP_PUSH_BEFORE || op == OP_PUSH_AFTER ||
                      op == OP_DEL_BEFORE || op == OP_DEL_AFTER);

    if (hasTarget && sFind(&list, t[2]) < 0) {
        error = "Элемент '" + t[2] + "' не найден";
    } else {
        switch (op) {
            case OP_PUSH_HEAD:
                sAddHead(&list, t[2]);
                result = t[2];
                changed = true;
                break;
            case OP_PUSH_TAIL:
                sAddTail(&list, t[2]);
                result = t[2];
                changed = true;
                break;
            case OP_PUSH_BEFORE:
                ok = sAddBefore(&list, t[2], t[3]);
                result = t[3];
                changed = ok;
                break;
            case OP_PUSH_AFTER:
                ok = sAddAfter(&list, t[2], t[3]);
                result = t[3];
                changed = ok;
                break;
            case OP_DEL_HEAD:
                ok = sDelHead(&list, result);
                changed = ok;
                break;
            case OP_DEL_TAIL:
                ok = sDelTail(&list, result);
                changed = ok;
                break;
            case OP_DEL_BEFORE:
                ok = sDelBefore(&list, t[2], result);
                changed = ok;
                break;
            case OP_DEL_AFTER:
                ok = sDelAfter(&list, t[2], result);
                changed = ok;
                break;
            case OP_DEL_VALUE:
                changed = sDelValue(&list, t[2]);
                result = changed ? "TRUE" : "FALSE";
                break;
            case OP_FIND: {
                int position = sFind(&list, t[2]);
                result = (position < 0) ? "NOT_FOUND" : std::to_string(position);
                break;
            }
            case OP_GET:
                if (!parseIndex(t[2], index)) {
                    error = "Неверный индекс";
                } else if (!sGet(&list, index, result)) {
                    error = "Индекс вне диапазона";
                }
                break;
            case OP_GET_HEAD:
                ok = sGetHead(&list, result);
                break;
            case OP_GET_TAIL:
                ok = sGetTail(&list, result);
                break;
            default:
                break;
        }
    }
    if (error == "" && !ok) {
        error = "Такого элемента нет";
    }

    if (error == "" && changed) {
        values.clear();
        sToValues(&list, values);
    }
    freeSList(&list);
    return error;
}

static std::string doDList(Op op, const std::vector<std::string>& t,
                           std::vector<std::string>& values, std::string& result, bool& changed) {
    DList list;
    initDList(&list);
    for (size_t i = 0; i < values.size(); i++) {
        dAddTail(&list, values[i]);
    }

    std::string error = "";
    int index = 0;
    bool ok = true;
    bool hasTarget = (op == OP_PUSH_BEFORE || op == OP_PUSH_AFTER ||
                      op == OP_DEL_BEFORE || op == OP_DEL_AFTER);

    if (hasTarget && dFind(&list, t[2]) < 0) {
        error = "element '" + t[2] + "' not found";
    } else {
        switch (op) {
            case OP_PUSH_HEAD:
                dAddHead(&list, t[2]);
                result = t[2];
                changed = true;
                break;
            case OP_PUSH_TAIL:
                dAddTail(&list, t[2]);
                result = t[2];
                changed = true;
                break;
            case OP_PUSH_BEFORE:
                ok = dAddBefore(&list, t[2], t[3]);
                result = t[3];
                changed = ok;
                break;
            case OP_PUSH_AFTER:
                ok = dAddAfter(&list, t[2], t[3]);
                result = t[3];
                changed = ok;
                break;
            case OP_DEL_HEAD:
                ok = dDelHead(&list, result);
                changed = ok;
                break;
            case OP_DEL_TAIL:
                ok = dDelTail(&list, result);
                changed = ok;
                break;
            case OP_DEL_BEFORE:
                ok = dDelBefore(&list, t[2], result);
                changed = ok;
                break;
            case OP_DEL_AFTER:
                ok = dDelAfter(&list, t[2], result);
                changed = ok;
                break;
            case OP_DEL_VALUE:
                changed = dDelValue(&list, t[2]);
                result = changed ? "TRUE" : "FALSE";
                break;
            case OP_FIND: {
                int position = dFind(&list, t[2]);
                result = (position < 0) ? "NOT_FOUND" : std::to_string(position);
                break;
            }
            case OP_GET:
                if (!parseIndex(t[2], index)) {
                    error = "Неверный индекс";
                } else if (!dGet(&list, index, result)) {
                    error = "Индекс вне диапозона";
                }
                break;
            case OP_GET_HEAD:
                ok = dGetHead(&list, result);
                break;
            case OP_GET_TAIL:
                ok = dGetTail(&list, result);
                break;
            default:
                break;
        }
    }
    if (error == "" && !ok) {
        error = "Такого элемента нет";
    }

    if (error == "" && changed) {
        values.clear();
        dToValues(&list, values);
    }
    freeDList(&list);
    return error;
}

static std::string doStack(Op op, const std::vector<std::string>& t,
                           std::vector<std::string>& values, std::string& result, bool& changed) {
    Stack stack;
    initStack(&stack);
    for (int i = (int)values.size() - 1; i >= 0; i--) {
        stackPush(&stack, values[i]);  
    }

    std::string error = "";
    bool ok = true;

    switch (op) {
        case OP_PUSH:
            stackPush(&stack, t[2]);
            result = t[2];
            changed = true;
            break;
        case OP_POP:
            ok = stackPop(&stack, result);
            changed = ok;
            break;
        case OP_PEEK:
            ok = stackPeek(&stack, result);
            break;
        default:
            break;
    }
    if (!ok) {
        error = "Стек пуст";
    }

    if (error == "" && changed) {
        values.clear();
        stackToValues(&stack, values);
    }
    freeStack(&stack);
    return error;
}

static std::string doQueue(Op op, const std::vector<std::string>& t,
                           std::vector<std::string>& values, std::string& result, bool& changed) {
    Queue queue;
    initQueue(&queue);
    for (size_t i = 0; i < values.size(); i++) {
        queuePush(&queue, values[i]);
    }

    std::string error = "";
    bool ok = true;

    switch (op) {
        case OP_PUSH:
            queuePush(&queue, t[2]);
            result = t[2];
            changed = true;
            break;
        case OP_POP:
            ok = queuePop(&queue, result);
            changed = ok;
            break;
        case OP_PEEK:
            ok = queuePeek(&queue, result);
            break;
        default:
            break;
    }
    if (!ok) {
        error = "Очередь пуста";
    }

    if (error == "" && changed) {
        values.clear();
        queueToValues(&queue, values);
    }
    freeQueue(&queue);
    return error;
}

static std::string doTree(Op op, const std::vector<std::string>& t,
                          std::vector<std::string>& values, std::string& result, bool& changed) {
    Tree tree;
    initTree(&tree);
    for (size_t i = 0; i < values.size(); i++) {
        treeInsert(&tree, values[i]);
    }

    switch (op) {
        case OP_PUSH:  
            treeInsert(&tree, t[2]);
            result = t[2];
            changed = true;
            values.clear();
            treeToValues(&tree, values);
            break;
        case OP_FIND:  
            result = treeFind(&tree, t[2]) ? "TRUE" : "FALSE";
            break;
        case OP_IS_COMPLETE:
            result = treeIsComplete(&tree) ? "TRUE" : "FALSE";
            break;
        default:
            break;
    }

    freeTree(&tree);
    return "";
}

static void printStructure(char type, const std::string& name, const std::vector<std::string>& values) {
    std::cout << "[" << type << "] " << name << ":" << std::endl;

    switch (type) {
        case 'M': {
            Array arr;
            initArray(&arr);
            for (size_t i = 0; i < values.size(); i++) arrayPush(&arr, values[i]);
            printArray(&arr);
            freeArray(&arr);
            break;
        }
        case 'F': {
            SList list;
            initSList(&list);
            for (int i = (int)values.size() - 1; i >= 0; i--) sAddHead(&list, values[i]);
            printSList(&list);
            freeSList(&list);
            break;
        }
        case 'L': {
            DList list;
            initDList(&list);
            for (size_t i = 0; i < values.size(); i++) dAddTail(&list, values[i]);
            printDList(&list);
            freeDList(&list);
            break;
        }
        case 'S': {
            Stack stack;
            initStack(&stack);
            for (int i = (int)values.size() - 1; i >= 0; i--) stackPush(&stack, values[i]);
            printStack(&stack);
            freeStack(&stack);
            break;
        }
        case 'Q': {
            Queue queue;
            initQueue(&queue);
            for (size_t i = 0; i < values.size(); i++) queuePush(&queue, values[i]);
            printQueue(&queue);
            freeQueue(&queue);
            break;
        }
        case 'T': {
            Tree tree;
            initTree(&tree);
            for (size_t i = 0; i < values.size(); i++) treeInsert(&tree, values[i]);
            printTree(&tree);
            freeTree(&tree);
            break;
        }
        default:
            break;
    }
}

bool runQuery(const std::string& file, const std::string& query) {
    std::vector<std::string> t = split(query);
    if (t.empty()) {
        return fail("Пустой запрос");
    }
    t[0] = toUpper(t[0]);

    const Command* cmd = findCommand(t[0]);
    if (cmd == nullptr) {
        return fail("Неизвестная команда: " + t[0]);
    }
    if ((int)t.size() != cmd->args + 1) {
        return fail("Неправильное количество аргументов для" + t[0]);
    }

    std::vector<std::string> lines = readLines(file);
    std::string name = t[1];
    int lineIndex = findLine(lines, name);

    if (lineIndex < 0 && !cmd->creates) {
        return fail("Структура '" + name + "' не найдена");
    }
    if (lineIndex >= 0 && cmd->op == OP_CREATE) {
        return fail("Массив уже существует");
    }

    char type = cmd->type;
    std::vector<std::string> values;
    if (lineIndex >= 0) {
        std::vector<std::string> fields = split(lines[lineIndex]);
        if (cmd->op == OP_PRINT) {
            type = fields[0][0];  
        } else if (fields[0][0] != type) {
            return fail("Имя '" + name + "' принадлежит к другому типу структуры");
        }
        values.assign(fields.begin() + 2, fields.end());
    }

    if (cmd->op == OP_PRINT) {
        printStructure(type, name, values);
        return true;
    }

    std::string result;
    std::string error;
    bool changed = false;

    switch (type) {
        case 'M': error = doArray(cmd->op, t, values, result, changed); break;
        case 'F': error = doSList(cmd->op, t, values, result, changed); break;
        case 'L': error = doDList(cmd->op, t, values, result, changed); break;
        case 'S': error = doStack(cmd->op, t, values, result, changed); break;
        case 'Q': error = doQueue(cmd->op, t, values, result, changed); break;
        case 'T': error = doTree(cmd->op, t, values, result, changed); break;
        default: break;
    }

    if (error != "") {
        return fail(error);
    }
    std::cout << "-> " << result << std::endl;

    if (changed) {
        std::string line = std::string(1, type) + " " + name;
        for (size_t i = 0; i < values.size(); i++) {
            line += " " + values[i];
        }
        if (lineIndex >= 0) {
            lines[lineIndex] = line;
        } else {
            lines.push_back(line);
        }
        if (!writeLines(file, lines)) {
            return fail("Не удается записать файл " + file);
        }
    }
    return true;
}