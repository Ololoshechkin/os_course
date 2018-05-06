//
// Created by Vadim on 05/05/2018.
//

#ifndef JIT_CODEWITHCYCLES_H
#define JIT_CODEWITHCYCLES_H

#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <sys/mman.h>
#include <string>
#include "Memory.h"
#include <functional>

typedef int (* IntProducer)();

struct CodeWithCycles {
  explicit CodeWithCycles(
          const std::vector<byte_type>& cycled_code_base
  );
  int Execute();
  void FindForLoopsAndUnwrap();
 private:
  struct OptionalByte {
    bool is_present;
    byte_type byte;
    bool Test(byte_type byte_to_check) const;
  };
  
  std::vector<byte_type> GetUnwrappedCode();
  static const std::vector<OptionalByte> for_bound_check_pattern;
  static const std::vector<OptionalByte> out_of_for_jump_pattern;
  static const std::vector<OptionalByte> jump_pattern;
  template<typename F>
  friend size_t FindEntranceAndGetOffset(
          const std::vector<byte_type>& code,
          const std::vector<CodeWithCycles::OptionalByte>& pattern,
          size_t code_offset,
          F action_with_managing_byte
  );
  Memory memory;
  std::vector<byte_type> current_code;
};

#endif //JIT_CODEWITHCYCLES_H
