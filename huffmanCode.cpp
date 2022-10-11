//
// Created by zribktad  on 10/4/2022.
//

#include "huffmanCode.h"


//
// Created by zribktad.
//

#include <queue>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <atomic>
#include "huffmanCode.h"

using namespace std;


Node::Node(unsigned char l, std::size_t count) : letter(l), count(count) {
    left = nullptr;
    right = nullptr;
    is_letter = true;
}

Node::Node(std::size_t count, Node *left, Node *right) : count(count), left(left), right(right) {
    letter = 1;
    is_letter = false;
}

void print2DUtil(Node *root, int space, std::string code = "") {
    if (root == nullptr)
        return;
    space += 7;
    print2DUtil(root->right, space, "1");
    for (int i = 7; i < space; i++)
        std::cout << " ";
    std::cout << root->letter << ": " << root->count << "(" << code << ")\n";
    print2DUtil(root->left, space, "0");
}


void huffmanCode::TreeShow(Node *node) {
    if (!node)
        return;

    std::cout << "name: " << node << " letter: " << node->letter << " c: " << node->count << " left:" << node->left
              << " right: " << node->right << std::endl;
    print2DUtil(node, 0);
}

void huffmanCode::GetCodes(Node *node,  std::map<unsigned char, string>& codex, std::string code) {
    if (!node)
        return;
    GetCodes(node->right, codex, code + "1");
    if (node->is_letter) {
       codex.insert(pair<unsigned char,string>(node->letter, code));
    }
    GetCodes(node->left, codex, code + "0");
}

Node * huffmanCode::huffmanTree(unordered_map<unsigned char,std::atomic<int>> &countForLetter ) {
    std::priority_queue<Node *, std::vector<Node *>, CmpNodePtrs> queue;
    for (auto iterator = countForLetter.begin(); iterator != countForLetter.end(); ++iterator) {
        Node *node = new Node(iterator->first, iterator->second);
        queue.push(node);

    }
    Node *first, *second, *root;
    if(queue.size()<=1){
        first = queue.top();
        return new Node(first->count,nullptr,first);
    }

    while (queue.size() != 1) {
        first = queue.top();
        queue.pop();

        second = queue.top();
        queue.pop();
        root = new Node(first->count + second->count, first, second);
        queue.push(root);

    }
    return queue.top();
}


void huffmanCode::removeTree(Node *root) {
if(root){
    removeTree(root->left);
    removeTree(root->right);
    delete root;
}

}
