#pragma once

#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <unordered_map>
#include "trie.h"

constexpr int kMaxPathLength = 4096 / sizeof(char);
constexpr int kMaxComponentLength = 256 / sizeof(char);
using Stat = struct stat;

struct StatWrapper {
    Stat stat;
    bool is_invalid;
    static StatWrapper FromStat(Stat);
    static const StatWrapper invalid_stat;
};

struct FileSystemNode : public std::initializer_list<::FileSystemNode> {
    std::string name_;
    FileSystemNode(const std::string& path);
    FileSystemNode();
    operator std::string();
    bool IsFile() const;
    bool operator<(const FileSystemNode& other) const;
    bool IsInvalid() const;
    ino_t GetInode() const;
    StatWrapper GetStat() const;
    std::vector<FileSystemNode> GetEdges() const;
    bool ContainsPattern(Trie& automaton) const;
};