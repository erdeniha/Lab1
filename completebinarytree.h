#pragma once

#include <string>
#include <vector>

struct TreeNode {
    std::string value;
    TreeNode* left;
    TreeNode* right;
};

struct Tree {
    TreeNode* root;
    int size;
};

void initTree(Tree* tree);
void freeTree(Tree* tree);
void treeInsert(Tree* tree, const std::string& value);
bool treeFind(const Tree* tree, const std::string& value);
bool treeIsComplete(const Tree* tree);
void treeToValues(const Tree* tree, std::vector<std::string>& values);  
void printTree(const Tree* tree);

