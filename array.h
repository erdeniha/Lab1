#pragma once
#include <string>
#include <vector>

struct Array {
    std::string* data;
    int size;
    int capacity;
};

void initArray(Array* arr);
void freeArray(Array* arr);
void arrayPush(Array* arr, const std::string& value);
bool arrayInsert(Array* arr, int index, const std::string& value);
bool arrayGet(const Array* arr, int index, std::string& result);
bool arrayDel(Array* arr, int index, std::string& result);
bool arraySet(Array* arr, int index, const std::string& value);
int arrayLength(const Array* arr);
void arrayToValues(const Array* arr, std::vector<std::string>& values);
void printArray(const Array* arr);

