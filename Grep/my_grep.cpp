#include <cstring>
#include <dirent.h>
#include <iostream>
#include <unistd.h>
#include <map>
#include <set>
#include "trie.h"
#include "fs_utils.h"
#include <algorithm>

std::set<FileSystemNode> GetCanonicalPaths(const FileSystemNode& folder) {
    std::map<ino_t, FileSystemNode> indode_to_shortest_path;
    std::set<FileSystemNode> shortest_paths;
    std::set<FileSystemNode> q;
    q.insert(folder);
    auto update = [&indode_to_shortest_path, &shortest_paths, &q](FileSystemNode const& child){
        q.insert(child);
        indode_to_shortest_path[child.GetInode()] = child;
        shortest_paths.insert(child);
    };
    while (!q.empty()) {
        auto cur_node = *q.begin();
        q.erase(cur_node);
        if (cur_node.IsInvalid())
            continue;
        for (const auto& child : cur_node.GetEdges()) {
            if (!indode_to_shortest_path.count(child.GetInode())) {
                update(child);
            } else if (child < indode_to_shortest_path[child.GetInode()]) {
                shortest_paths.erase(indode_to_shortest_path[child.GetInode()]);
                update(child);
            }
        }
    }
    return shortest_paths;
}

void DoGrep(char* pattern) {
    char current_dir[kMaxPathLength];
    if (getcwd(current_dir, sizeof(current_dir)) != nullptr) {
        Trie pattern_checker;
        pattern_checker.AddWord(pattern);
        pattern_checker.Build();
        auto nodes = GetCanonicalPaths(FileSystemNode(current_dir));
        std::for_each(nodes.begin(), nodes.end(), [&pattern_checker](FileSystemNode const& cur_node) {
            if (cur_node.ContainsPattern(pattern_checker))
                write(STDOUT_FILENO, (cur_node.name_ + "\n").c_str(),
                      cur_node.name_.size() + 1);
        });
    } else {
        perror("failed to get current directory pathname");
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout
                << "expected single argument - word to grep from current folder";
        return 0;
    }
    DoGrep(argv[1]);
    return 0;
}
