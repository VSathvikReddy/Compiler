#include <iostream>
#include <string>

#include "symbol_trie.hpp"

SymbolTrie::Node::Node() {
    for (int i = 0; i < 256; ++i) children[i] = nullptr;
}
SymbolTrie::Node::~Node(){
    for (int i = 0; i < 256; ++i) {
        if(children[i]){
            delete children[i];
        }
    }
}

SymbolTrie::SymbolTrie() {
    this->root = new Node();
}

SymbolTrie::~SymbolTrie() {
    delete root;
}

void SymbolTrie::insert(const std::string& key) {
    Node* curr = root;

    for (char c : key) {
        unsigned char idx = static_cast<unsigned char>(c);

        if (curr->children[idx] == nullptr) {
            curr->children[idx] = new Node();
        }
        
        curr = curr->children[idx];
    }

    curr->isLeaf = true;
}

bool SymbolTrie::search(const std::string& key) const {
    const Node* curr = root;

    for (char c : key) {
        unsigned char idx = static_cast<unsigned char>(c);
        if (curr->children[idx] == nullptr) {
            return false;
        }
        curr = curr->children[idx];
    }
    return curr->isLeaf;
}