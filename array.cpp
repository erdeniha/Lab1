#include "array.h"

#include <iostream>

void initArray(Array* arr) {
    arr->capacity = 4;
    arr->size = 0;
    arr->data = new std::string[arr->capacity];
}

void freeArray(Array* arr) {
    delete[] arr->data;
    arr->data = nullptr;
    arr->size = 0;
}

static void growArray(Array* arr) {
    int newCapacity = arr->capacity * 2;
    std::string* newData = new std::string[newCapacity];
    for (int i = 0; i < arr->size; i++) {
        newData[i] = arr->data[i];
    }
    delete[] arr->data;
    arr->data = newData;
    arr->capacity = newCapacity;
}

void arrayPush(Array* arr, const std::string& value) {
    if (arr->size == arr->capacity) {
        growArray(arr);
    }
    arr->data[arr->size] = value;
    arr->size++;
}

bool arrayInsert(Array* arr, int index, const std::string& value) {
    if (index < 0 || index > arr->size) {
        return false;
    }
    if (arr->size == arr->capacity) {
        growArray(arr);
    }
    for (int i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i - 1];
    }
    arr->data[index] = value;
    arr->size++;
    return true;
}

bool arrayGet(const Array* arr, int index, std::string& result) {
    if (index < 0 || index >= arr->size) {
        return false;
    }
    result = arr->data[index];
    return true;
}

bool arrayDel(Array* arr, int index, std::string& result) {
    if (index < 0 || index >= arr->size) {
        return false;
    }
    result = arr->data[index];
    for (int i = index; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
    return true;
}

bool arraySet(Array* arr, int index, const std::string& value) {
    if (index < 0 || index >= arr->size) {
        return false;
    }
    arr->data[index] = value;
    return true;
}

int arrayLength(const Array* arr) {
    return arr->size;
}

void arrayToValues(const Array* arr, std::vector<std::string>& values) {
    for (int i = 0; i < arr->size; i++) {
        values.push_back(arr->data[i]);
    }
}

void printArray(const Array* arr) {
    std::cout << "[";
    for (int i = 0; i < arr->size; i++) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << arr->data[i];
    }
    std::cout << "] (size=" << arr->size << ")" << std::endl;
}