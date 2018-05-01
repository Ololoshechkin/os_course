//
// Created by Vadim on 15/04/2018.
//

#include "InetUtils.h"
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

const std::string InetUtils::kLoopbackAddress = "127.0.0.1";
const int InetUtils::kDefaultBacklogSize = 1;