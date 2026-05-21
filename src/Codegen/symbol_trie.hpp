#include <iostream>
#include <string>

class SymbolTrie {
private:
    struct Node {
        Node* children[256];
        bool isLeaf = false;
        Node();
        ~Node();
    };
    Node* root = nullptr;


public:
    SymbolTrie();
    ~SymbolTrie();
    SymbolTrie(const SymbolTrie&) = delete;
    SymbolTrie& operator=(const SymbolTrie&) = delete;

    void insert(const std::string& key);
    bool search(const std::string& key) const;
};