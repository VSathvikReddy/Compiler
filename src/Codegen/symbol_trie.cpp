#include <iostream>
#include <string>

#include "symbol_trie.hpp"

SymbolTrie::Node::Node() {}
SymbolTrie::Node::~Node(){
    for (auto itr: children) {
        if(itr.second){
            delete itr.second;
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

        if (!curr->children[c]) {
            curr->children[c] = new Node();
        }
        
        curr = curr->children[c];
    }

    curr->isLeaf = true;
}

bool SymbolTrie::search(const std::string& key) const {
    const Node* curr = root;

    for (char c : key) {
        auto it = curr->children.find(c);
        if (it == curr->children.end()) {
            return false;
        }
        curr = it->second;
    }
    return curr->isLeaf;
}