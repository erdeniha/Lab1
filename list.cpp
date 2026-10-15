#include "list.h"

#include <iostream>

void initSList(SList* list) {
    list->head = nullptr;
}

void freeSList(SList* list) {
    while (list->head != nullptr) {
        Node* temp = list->head;
        list->head = temp->next;
        delete temp;
    }
}

void sAddHead(SList* list, const std::string& value) {
    list->head = new Node{value, list->head};
}


static Node* findSNode(const SList* list, const std::string& value) {
    Node* current = list->head;
    while (current != nullptr && current->value != value) {
        current = current->next;
    }
    return current;
}

bool sAddBefore(SList* list, const std::string& target, const std::string& value) {
    if (list->head == nullptr) {
        return false;
    }
    if (list->head->value == target) {
        sAddHead(list, value); 
        return true;
    }
    Node* current = list->head;
    while (current->next != nullptr && current->next->value != target) {
        current = current->next;
    }
    if (current->next == nullptr) {
        return false;  
    }
    current->next = new Node{value, current->next};
    return true;
}


bool sAddAfter(SList* list, const std::string& target, const std::string& value) {
    Node* found = findSNode(list, target);
    if (found == nullptr) {
        return false;
    }
    found->next = new Node{value, found->next};
    return true;
}

void sAddTail(SList* list, const std::string& value) {
    Node* newNode = new Node{value, nullptr};
    if (list->head == nullptr) {
        list->head = newNode;
        return;
    }
    Node* current = list->head;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = newNode;
}

bool sDelHead(SList* list, std::string& result) {
    if (list->head == nullptr) {
        return false;
    }
    Node* temp = list->head;
    result = temp->value;
    list->head = temp->next;
    delete temp;
    return true;
}

bool sDelBefore(SList* list, const std::string& target, std::string& result) {
    if (list->head == nullptr || list->head->next == nullptr || list->head->value == target) {
        return false;
    }
    if (list->head->next->value == target) {
        return sDelHead(list, result);  
    }
    Node* current = list->head;
    while (current->next->next != nullptr && current->next->next->value != target) {
        current = current->next;
    }
    if (current->next->next == nullptr) {
        return false;  
    }
    Node* temp = current->next;
    result = temp->value;
    current->next = temp->next;
    delete temp;
    return true;
}

bool sDelAfter(SList* list, const std::string& target, std::string& result) {
    Node* found = findSNode(list, target);
    if (found == nullptr || found->next == nullptr) {
        return false;
    }
    Node* temp = found->next;
    result = temp->value;
    found->next = temp->next;
    delete temp;
    return true;
}

bool sDelTail(SList* list, std::string& result) {
    if (list->head == nullptr) {
        return false;
    }
    if (list->head->next == nullptr) {
        return sDelHead(list, result);
    }
    Node* current = list->head;
    while (current->next->next != nullptr) {
        current = current->next;
    }
    result = current->next->value;
    delete current->next;
    current->next = nullptr;
    return true;
}

bool sDelValue(SList* list, const std::string& value) {
    Node* prev = nullptr;
    Node* current = list->head;
    while (current != nullptr && current->value != value) {
        prev = current;
        current = current->next;
    }
    if (current == nullptr) {
        return false;
    }
    if (prev == nullptr) {
        list->head = current->next;
    } else {
        prev->next = current->next;
    }
    delete current;
    return true;
}

int sFind(const SList* list, const std::string& value) {
    int index = 0;
    for (Node* current = list->head; current != nullptr; current = current->next) {
        if (current->value == value) {
            return index;
        }
        index++;
    }
    return -1;
}

bool sGet(const SList* list, int index, std::string& result) {
    if (index < 0) {
        return false;
    }
    Node* current = list->head;
    while (current != nullptr && index > 0) {
        current = current->next;
        index--;
    }
    if (current == nullptr) {
        return false;
    }
    result = current->value;
    return true;
}

bool sGetHead(const SList* list, std::string& result) {
    if (list->head == nullptr) {
        return false;
    }
    result = list->head->value;
    return true;
}

bool sGetTail(const SList* list, std::string& result) {
    if (list->head == nullptr) {
        return false;
    }
    Node* current = list->head;
    while (current->next != nullptr) {
        current = current->next;
    }
    result = current->value;
    return true;
}

void sToValues(const SList* list, std::vector<std::string>& values) {
    for (Node* current = list->head; current != nullptr; current = current->next) {
        values.push_back(current->value);
    }
}

void printSList(const SList* list) {
    for (Node* current = list->head; current != nullptr; current = current->next) {
        std::cout << current->value << " -> ";
    }
    std::cout << "NULL" << std::endl;
}

//ниже двусвязный список

void initDList(DList* list) {
    list->head = nullptr;
    list->tail = nullptr;
}

void freeDList(DList* list) {
    while (list->head != nullptr) {
        DNode* temp = list->head;
        list->head = temp->next;
        delete temp;
    }
    list->tail = nullptr;
}

static void unlinkNode(DList* list, DNode* node) {
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    delete node;
}

void dAddHead(DList* list, const std::string& value) {
    DNode* node = new DNode{value, nullptr, list->head};
    if (list->head != nullptr) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }
    list->head = node;
}

void dAddTail(DList* list, const std::string& value) {
    DNode* node = new DNode{value, list->tail, nullptr};
    if (list->tail != nullptr) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
}

static DNode* findDNode(const DList* list, const std::string& value) {
    DNode* current = list->head;
    while (current != nullptr && current->value != value) {
        current = current->next;
    }
    return current;
}

bool dAddBefore(DList* list, const std::string& target, const std::string& value) {
    DNode* found = findDNode(list, target);
    if (found == nullptr) {
        return false;
    }
    if (found->prev == nullptr) {
        dAddHead(list, value);  
        return true;
    }
    DNode* node = new DNode{value, found->prev, found};
    found->prev->next = node;
    found->prev = node;
    return true;
}

bool dAddAfter(DList* list, const std::string& target, const std::string& value) {
    DNode* found = findDNode(list, target);
    if (found == nullptr) {
        return false;
    }
    if (found->next == nullptr) {
        dAddTail(list, value);  
        return true;
    }
    DNode* node = new DNode{value, found, found->next};
    found->next->prev = node;
    found->next = node;
    return true;
}

bool dDelHead(DList* list, std::string& result) {
    if (list->head == nullptr) {
        return false;
    }
    result = list->head->value;
    unlinkNode(list, list->head);
    return true;
}

bool dDelBefore(DList* list, const std::string& target, std::string& result) {
    DNode* found = findDNode(list, target);
    if (found == nullptr || found->prev == nullptr) {
        return false; 
    }
    result = found->prev->value;
    unlinkNode(list, found->prev);
    return true;
}

bool dDelAfter(DList* list, const std::string& target, std::string& result) {
    DNode* found = findDNode(list, target);
    if (found == nullptr || found->next == nullptr) {
        return false; 
    }
    result = found->next->value;
    unlinkNode(list, found->next);
    return true;
}

bool dDelTail(DList* list, std::string& result) {
    if (list->tail == nullptr) {
        return false;
    }
    result = list->tail->value;
    unlinkNode(list, list->tail);
    return true;
}

bool dDelValue(DList* list, const std::string& value) {
    for (DNode* current = list->head; current != nullptr; current = current->next) {
        if (current->value == value) {
            unlinkNode(list, current);
            return true;
        }
    }
    return false;
}

int dFind(const DList* list, const std::string& value) {
    int index = 0;
    for (DNode* current = list->head; current != nullptr; current = current->next) {
        if (current->value == value) {
            return index;
        }
        index++;
    }
    return -1;
}

bool dGet(const DList* list, int index, std::string& result) {
    if (index < 0) {
        return false;
    }
    DNode* current = list->head;
    while (current != nullptr && index > 0) {
        current = current->next;
        index--;
    }
    if (current == nullptr) {
        return false;
    }
    result = current->value;
    return true;
}

bool dGetHead(const DList* list, std::string& result) {
    if (list->head == nullptr) {
        return false;
    }
    result = list->head->value;
    return true;
}

bool dGetTail(const DList* list, std::string& result) {
    if (list->tail == nullptr) {
        return false;
    }
    result = list->tail->value;
    return true;
}

void dToValues(const DList* list, std::vector<std::string>& values) {
    for (DNode* current = list->head; current != nullptr; current = current->next) {
        values.push_back(current->value);
    }
}

void printDList(const DList* list) {
    std::cout << "NULL";
    for (DNode* current = list->head; current != nullptr; current = current->next) {
        std::cout << " <-> " << current->value;
    }
    std::cout << " <-> NULL" << std::endl;
}