#pragma once

#include <string>
#include <vector>

#include "node.h"

struct Stack {
    Node* head;  
};

void initStack(Stack* stack);
void freeStack(Stack* stack);
void stackPush(Stack* stack, const std::string& value);
bool stackPop(Stack* stack, std::string& result);
bool stackPeek(const Stack* stack, std::string& result);
void stackToValues(const Stack* stack, std::vector<std::string>& values);
void printStack(const Stack* stack);
