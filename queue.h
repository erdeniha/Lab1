#pragma once
#include <string>
#include <vector>

#include "node.h"

struct Queue {
    Node* head;  
    Node* tail;  
};

void initQueue(Queue* queue);
void freeQueue(Queue* queue);
void queuePush(Queue* queue, const std::string& value);
bool queuePop(Queue* queue, std::string& result);
bool queuePeek(const Queue* queue, std::string& result);
void queueToValues(const Queue* queue, std::vector<std::string>& values);  
void printQueue(const Queue* queue);
