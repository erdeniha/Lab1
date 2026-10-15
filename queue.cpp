#include "queue.h"

#include <iostream>

void initQueue(Queue* queue) {
    queue->head = nullptr;
    queue->tail = nullptr;
}

void freeQueue(Queue* queue) {
    while (queue->head != nullptr) {
        Node* temp = queue->head;
        queue->head = temp->next;
        delete temp;
    }
    queue->tail = nullptr;
}

void queuePush(Queue* queue, const std::string& value) {
    Node* newNode = new Node{value, nullptr};
    if (queue->tail == nullptr) {
        queue->head = newNode;
    } else {
        queue->tail->next = newNode;
    }
    queue->tail = newNode;
}

bool queuePop(Queue* queue, std::string& result) {
    if (queue->head == nullptr) {
        return false;
    }
    Node* temp = queue->head;
    result = temp->value;
    queue->head = temp->next;
    if (queue->head == nullptr) {
        queue->tail = nullptr;
    }
    delete temp;
    return true;
}

bool queuePeek(const Queue* queue, std::string& result) {
    if (queue->head == nullptr) {
        return false;
    }
    result = queue->head->value;
    return true;
}

void queueToValues(const Queue* queue, std::vector<std::string>& values) {
    for (Node* current = queue->head; current != nullptr; current = current->next) {
        values.push_back(current->value);
    }
}

void printQueue(const Queue* queue) {
    if (queue->head == nullptr) {
        std::cout << "(Пустая очередь)" << std::endl;
        return;
    }
    std::cout << "front ->";
    for (Node* current = queue->head; current != nullptr; current = current->next) {
        std::cout << " " << current->value;
    }
    std::cout << " <- back" << std::endl;
}