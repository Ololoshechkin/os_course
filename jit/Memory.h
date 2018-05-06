//
// Created by Vadim on 05/05/2018.
//

#ifndef JIT_MEMORY_H
#define JIT_MEMORY_H

#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <sys/mman.h>
#include <string>

using flag_type = decltype(PROT_NONE);
using byte_type = unsigned char;

struct Memory {
  enum Rights {
    kRead, kWrite, kExecute, kNone
  };
  explicit Memory(size_t size);
  void SetRights(const std::vector<Rights>& rights);
  void FillWithBytes(const std::vector<byte_type>& bytes);
  
  template<typename T>
  T CastTo() {
    return (T) ptr;
  }
  
  ~Memory();
 private:
  static flag_type RightsConvert(Rights rights) noexcept;
  static flag_type RightsConvert(const std::vector<Rights>& rights) noexcept;
  size_t size;
  void* ptr;
  static const flag_type kFlags;
  static const flag_type kDefaultRights;
};

#endif //JIT_MEMORY_H
