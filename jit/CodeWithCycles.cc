//
// Created by Vadim on 05/05/2018.
//

#include "CodeWithCycles.h"

const std::vector<
        CodeWithCycles::OptionalByte
> CodeWithCycles::for_bound_check_pattern = { // NOLINT
        {true,  0x83},
        {true,  0x7d},
        {true,  0xf8},
        {false, 0x05} //5
        // 	cmpl	$5, -8(%rbp)
};
const std::vector<
        CodeWithCycles::OptionalByte
> CodeWithCycles::out_of_for_jump_pattern = { // NOLINT
        {true,  0x0f},
        {true,  0x8d},
        {false, 0x17}, // 23
        {true,  0x00},
        {true,  0x00},
        {true,  0x00},
        // jge	23 <_f+0x33>
};
const std::vector<
        CodeWithCycles::OptionalByte
> CodeWithCycles::jump_pattern = { // NOLINT
        {true,  0xe9},
        {false, 0xdf}, // -33 = 0xdf-0xff
        {true,  0xff},
        {true,  0xff},
        {true,  0xff}
        //	jmp	-33 <__Z1fv+0x12>
};

template<typename F>
size_t FindEntranceAndGetOffset(
        const std::vector<byte_type>& code,
        const std::vector<CodeWithCycles::OptionalByte>& pattern,
        size_t code_offset,
        F action_with_managing_byte
) {
  for (size_t i = code_offset; i <= code.size() - pattern.size(); ++i) {
    bool is_entrance = true;
    byte_type managing_byte = 0;
    for (size_t j = 0; j < pattern.size(); ++j) {
      if (!pattern[j].Test(code[i + j])) {
        is_entrance = false;
        break;
      }
      if (!pattern[j].is_present)
        managing_byte = code[i + j];
    }
    if (is_entrance) {
      action_with_managing_byte(managing_byte);
      return i + pattern.size();
    }
  }
  return code.size();
}

CodeWithCycles::CodeWithCycles(const std::vector<byte_type>& cycled_code_base) :
        memory(cycled_code_base.size()), current_code(cycled_code_base) {
  memory.FillWithBytes(current_code);
}

template<typename T, typename It>
void AddAll(std::vector<T>& dest, It src_start, It src_end) {
  for (auto it = src_start; it != src_end; ++it) {
    dest.push_back(*it);
  }
}

std::vector<byte_type> CodeWithCycles::GetUnwrappedCode() {
  decltype(current_code) result_code;
  size_t offset = 0;
  auto do_nothing = [](byte_type _) {};
  while (true) {
    int cycle_size = 0;
    size_t start_position = offset;
    offset = FindEntranceAndGetOffset(current_code, for_bound_check_pattern,
                                      offset,
                                      [&cycle_size](byte_type bound_byte) {
                                        cycle_size = static_cast<int>(bound_byte);
                                      });
    AddAll(result_code, current_code.begin() + start_position,
           current_code.begin() + offset);
    if (offset == current_code.size())
      break;
    size_t cycle_body_start = offset = FindEntranceAndGetOffset(current_code,
                                                                out_of_for_jump_pattern,
                                                                offset,
                                                                do_nothing);
    offset = FindEntranceAndGetOffset(current_code, jump_pattern, offset,
                                      do_nothing);
    size_t cycle_body_end = offset - jump_pattern.size();
    for (int i = 0; i < cycle_size; ++i) {
      AddAll(result_code, current_code.begin() + cycle_body_start,
             current_code.begin() + cycle_body_end);
    }
  }
  return result_code;
}

int CodeWithCycles::Execute() {
  auto func = memory.CastTo<IntProducer>();
  memory.SetRights({Memory::Rights::kRead, Memory::Rights::kExecute});
  int return_code = func();
  memory.SetRights({Memory::Rights::kRead, Memory::kWrite});
  return return_code;
}

void CodeWithCycles::FindForLoopsAndUnwrap() {
  current_code = GetUnwrappedCode();
  memory.FillWithBytes(current_code);
}

bool CodeWithCycles::OptionalByte::Test(byte_type byte_to_check) const {
  return !is_present || byte == byte_to_check;
}
