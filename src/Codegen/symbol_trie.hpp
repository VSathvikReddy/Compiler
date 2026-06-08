#include <iostream>
#include <string>
#include <map>

class SymbolTrie {
private:
    struct Node {
        std::map<char,Node*> children;
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