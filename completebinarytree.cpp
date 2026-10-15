#include "completebinarytree.h"

#include <iostream>

void initTree(Tree* tree) {
    tree->root = nullptr;
    tree->size = 0;
}

static void freeNode(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    freeNode(node->left);
    freeNode(node->right);
    delete node;
}

void freeTree(Tree* tree) {
    freeNode(tree->root);
    tree->root = nullptr;
    tree->size = 0;
}

static TreeNode* getNode(TreeNode* root, int index) {
    if (index == 0) {
        return root;
    }
    TreeNode* parent = getNode(root, (index - 1) / 2);
    if (index % 2 == 1) {
        return parent->left;   
    }
    return parent->right;      
}

void treeInsert(Tree* tree, const std::string& value) {
    TreeNode* newNode = new TreeNode{value, nullptr, nullptr};
    int index = tree->size;  
    tree->size++;
    if (index == 0) {
        tree->root = newNode;
        return;
    }
    TreeNode* parent = getNode(tree->root, (index - 1) / 2);
    if (index % 2 == 1) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
}

static bool findNode(const TreeNode* node, const std::string& value) {
    if (node == nullptr) {
        return false;
    }
    if (node->value == value) {
        return true;
    }
    return findNode(node->left, value) || findNode(node->right, value);
}

bool treeFind(const Tree* tree, const std::string& value) {
    return findNode(tree->root, value);
}


static bool checkComplete(const TreeNode* node, int index, int total) {
    if (node == nullptr) {
        return true;
    }
    if (index >= total) {
        return false;
    }
    return checkComplete(node->left, 2 * index + 1, total) &&
           checkComplete(node->right, 2 * index + 2, total);
}


bool treeIsComplete(const Tree* tree) {
    return checkComplete(tree->root, 0, tree->size);
}

void treeToValues(const Tree* tree, std::vector<std::string>& values) {
    for (int i = 0; i < tree->size; i++) {
        values.push_back(getNode(tree->root, i)->value);
    }
}
static void printSideways(const TreeNode* node, int depth) {
    if (node == nullptr) {
        return;
    }
    printSideways(node->right, depth + 1);
    std::cout << std::string(depth * 4, ' ') << node->value << std::endl;
    printSideways(node->left, depth + 1);
}

void printTree(const Tree* tree) {
    if (tree->root == nullptr) {
        std::cout << "(empty tree)" << std::endl;
        return;
    }
    std::vector<std::string> values;
    treeToValues(tree, values);
    std::cout << "Level order:";
    for (size_t i = 0; i < values.size(); i++) {
        std::cout << " " << values[i];
    }
    std::cout << std::endl;
    std::cout << "Дерево (повернутое на 90 градусов, корень слева, правый дочерний элемент сверху):" << std::endl;
    printSideways(tree->root, 0);
}