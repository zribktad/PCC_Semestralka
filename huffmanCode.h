//
// Created by zribktad  on 10/4/2022.
//

#ifndef SEM_HUFFMANCODE_H
#define SEM_HUFFMANCODE_H

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <atomic>


class Node {

public:
    unsigned char letter;
    std::size_t count;
    Node *left;
    Node *right;
    bool is_letter;

    Node(unsigned char l, std::size_t count);
    Node(std::size_t count, Node *left, Node *right);

};

typedef struct lcode{
    unsigned char letter;
    std::string code;
}lcode;


struct CmpNodePtrs
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return lhs->count > rhs->count;
    }
};


class huffmanCode {
public:
    static Node * huffmanTree(std::unordered_map<unsigned char,std::atomic<int>> &countForLetter );
    static void GetCodes(Node * node,  std::map<unsigned char, std::string>& codex , std::string code = "" );
    static void TreeShow(Node * node);
    static void removeTree(Node * root);

};


#endif //SEM_HUFFMANCODE_H
