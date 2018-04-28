#include <utility>
#include "fs_utils.h"
#include <vector>

StatWrapper StatWrapper::FromStat(Stat stat) {
    return {stat, false};
}

const StatWrapper StatWrapper::invalid_stat = {Stat{}, true};

FileSystemNode::FileSystemNode(const std::string& path) :
        name_(path) {
    while (name_.back() == '/') {
        name_.pop_back();
    }
}

FileSystemNode::FileSystemNode() :
        FileSystemNode("") {}

FileSystemNode::operator std::string() { return name_; }

bool FileSystemNode::IsFile() const { return S_ISREG(GetStat().stat.st_mode); }

bool FileSystemNode::operator<(const FileSystemNode& other) const {
    return name_.size() < other.name_.size() ||
           (name_.size() == other.name_.size() && name_ < other.name_);
}

bool FileSystemNode::IsInvalid() const {
    return GetStat().is_invalid || name_.length() > kMaxPathLength;
}

ino_t FileSystemNode::GetInode() const { return GetStat().stat.st_ino; }

StatWrapper FileSystemNode::GetStat() const {
    Stat st;
    if (stat(name_.c_str(), &st) == -1) {
        std::perror(("failed to get stats from : " + name_).c_str());
        return StatWrapper::invalid_stat;
    }
    return StatWrapper::FromStat(st);
}

std::vector<FileSystemNode> FileSystemNode::GetEdges() const {
    if (IsFile()) {
        return {};
    }
    auto dir = opendir(name_.c_str());
    if (dir != nullptr) {
        std::vector<FileSystemNode> edges;
        dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (strlen(entry->d_name) > kMaxComponentLength) {
                std::perror(
                        ("entry " + std::string(entry->d_name).substr(0, 100) +
                         "... is too large").c_str());
                continue;
            }
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
            }
            auto cur_path = name_ + "/" + std::string(entry->d_name);
            if (cur_path.size() > kMaxPathLength) {
                std::perror(("path " + cur_path.substr(0, 100) +
                             "... is too large").c_str());
                continue;
            }
            edges.emplace_back(cur_path);
        }
        closedir(dir);
        return edges;
    } else {
        std::perror(("failed to open directory : " + name_).c_str());
        return {};
    }
}

bool FileSystemNode::ContainsPattern(Trie& automaton) const {
    if (!IsFile())
        return false;
    auto session = automaton.StartAccepting();
    int file_descriptor = open(name_.c_str(), O_RDONLY);
    if (file_descriptor < 0) {
        return false;
    }
    char buf[1024];
    bool accepted = false;
    while (true) {
        auto read_count = read(file_descriptor, buf, 1024);
        if (read_count <= 0) {
            if (read_count != 0) {
                perror(("failed to read file: " + name_).c_str());
            }
            break;
        }
        for (decltype(read_count) i = 0; i < read_count; ++i) {
            if (session.AcceptNextSymbol(buf[i])) {
                accepted = true;
                break;
            }
        }
        if (accepted)
            break;
    }
    close(file_descriptor);
    return accepted;
}