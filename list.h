#pragma once

#include <string>
#include <vector>

#include "node.h"


struct SList {
    Node* head;
};

void initSList(SList* list);
void freeSList(SList* list);

void sAddHead(SList* list, const std::string& value);
bool sAddBefore(SList* list, const std::string& target, const std::string& value);
bool sAddAfter(SList* list, const std::string& target, const std::string& value);
void sAddTail(SList* list, const std::string& value);

bool sDelHead(SList* list, std::string& result);
bool sDelBefore(SList* list, const std::string& target, std::string& result);
bool sDelAfter(SList* list, const std::string& target, std::string& result);
bool sDelTail(SList* list, std::string& result);
bool sDelValue(SList* list, const std::string& value);

int sFind(const SList* list, const std::string& value);
bool sGet(const SList* list, int index, std::string& result);
bool sGetHead(const SList* list, std::string& result);
bool sGetTail(const SList* list, std::string& result);

void sToValues(const SList* list, std::vector<std::string>& values);
void printSList(const SList* list);

struct DNode {
    std::string value;
    DNode* prev;
    DNode* next;
};

struct DList {
    DNode* head;
    DNode* tail;
};

void initDList(DList* list);
void freeDList(DList* list);

void dAddHead(DList* list, const std::string& value);
bool dAddBefore(DList* list, const std::string& target, const std::string& value);
bool dAddAfter(DList* list, const std::string& target, const std::string& value);
void dAddTail(DList* list, const std::string& value);

bool dDelHead(DList* list, std::string& result);
bool dDelBefore(DList* list, const std::string& target, std::string& result);
bool dDelAfter(DList* list, const std::string& target, std::string& result);
bool dDelTail(DList* list, std::string& result);
bool dDelValue(DList* list, const std::string& value);

int dFind(const DList* list, const std::string& value);
bool dGet(const DList* list, int index, std::string& result);
bool dGetHead(const DList* list, std::string& result);
bool dGetTail(const DList* list, std::string& result);

void dToValues(const DList* list, std::vector<std::string>& values);
void printDList(const DList* list);
