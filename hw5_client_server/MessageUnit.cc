//
// Created by Vadim on 22/04/2018.
//

#include <sstream>
#include "MessageUnit.h"

size_t StringToInt(const std::string& s) {
  size_t int_val = 0;
  for (size_t i = 0; i < 4; ++i) {
    auto cur = static_cast<size_t>(s[i]);
    int_val = (int_val << 8) | cur;
  }
  return int_val;
}

std::string IntBytes(size_t length) {
  return {static_cast<char>((length >> 24) & 0xFF),
          static_cast<char>((length >> 16) & 0xFF),
          static_cast<char>((length >> 8) & 0xFF),
          static_cast<char>(length & 0xFF)};
}