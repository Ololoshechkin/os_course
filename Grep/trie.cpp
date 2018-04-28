#include <string>
#include <vector>
#include <queue>
#include "trie.h"
#include <limits>

// consts:
const int Trie::kZeroNode = 0;
const int Trie::kMinSymbol = std::numeric_limits<char>::min();
const int Trie::kMaxSymbol = std::numeric_limits<char>::max();

// private:
void Trie::NewLayer() {
    link_.push_back(kZeroNode);
    super_link_.push_back(kZeroNode);
    is_terminal_.push_back(kZeroNode);
    next_.emplace_back(kMaxSymbol - kMinSymbol);
}

bool Trie::IsTerminal(int vertex) const {
    return vertex && (is_terminal_[vertex] || IsTerminal(super_link_[vertex]));
}

size_t Trie::Size() const { return next_.size(); }

// public:
Trie::Trie() { NewLayer(); }

void Trie::AddWord(const std::string& word) {
    int vertex = kZeroNode;
    for (char c_char : word) {
        int c_index = c_char - kMinSymbol;
        if (!next_[vertex][c_index]) {
            NewLayer();
            next_[vertex][c_index] = static_cast<int>(Size() - 1);
        }
        vertex = next_[vertex][c_index];
    }
    is_terminal_[vertex] = true;
}

void Trie::Build() {
    std::queue<int> bfs_queue;
    bfs_queue.push(kZeroNode);
    while (!bfs_queue.empty()) {
        int vertex = bfs_queue.front();
        bfs_queue.pop();
        for (int symbol = 0; symbol <= kMaxSymbol - kMinSymbol; ++symbol) {
            if (!next_[vertex][symbol]) {
                next_[vertex][symbol] = next_[link_[vertex]][symbol];
            } else {
                int cur_vertex = next_[vertex][symbol];
                if (vertex) {
                    link_[cur_vertex] = next_[link_[vertex]][symbol];
                    super_link_[cur_vertex] = is_terminal_[link_[cur_vertex]]
                                              ? link_[cur_vertex]
                                              : super_link_[link_[cur_vertex]];
                }
                bfs_queue.push(cur_vertex);
            }
        }
    }
}

Trie::AcceptSession Trie::StartAccepting() const {
    return AcceptSession(this);
}

Trie::AcceptSession::AcceptSession(const Trie* trie) :
        vertex_(kZeroNode), trie_(trie) {}

bool Trie::AcceptSession::AcceptNextSymbol(char symbol) {
    vertex_ = trie_->next_[vertex_][symbol - kMinSymbol];
    int cur_vertex = vertex_;
    return trie_->IsTerminal(cur_vertex);
}

bool Trie::AcceptSession::Accept(const std::string& text) {
    vertex_ = kZeroNode;
    for (char symbol : text) {
        if (AcceptNextSymbol(symbol))
            return true;
    }
    return false;
}