#pragma once
#include <string>
#include <vector>
#include <queue>

class Trie {
 private:
    static const int kMinSymbol;
    static const int kMaxSymbol;
    static const int kZeroNode;
    std::vector<int> link_;
    std::vector<int> super_link_;
    std::vector<int> is_terminal_;
    std::vector<std::vector<int>> next_;
    void NewLayer();
    bool IsTerminal(int vertex) const;
    size_t Size() const;
 public:
    Trie();
    void AddWord(const std::string& word);
    void Build();
    
    struct AcceptSession {
        bool AcceptNextSymbol(char symbol);
        bool Accept(const std::string& text);
     private:
        explicit AcceptSession(const Trie* trie);
        int vertex_;
        const Trie* trie_;
        friend Trie;
    };
    
    AcceptSession StartAccepting() const;
    friend Trie::AcceptSession;
};
