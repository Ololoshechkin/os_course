//
// Created by Vadim on 05/05/2018.
//

#include "Memory.h"

const flag_type Memory::kFlags = MAP_PRIVATE | MAP_ANONYMOUS;
const flag_type Memory::kDefaultRights = PROT_READ | PROT_WRITE;

Memory::Memory(size_t size) :
        size(size), ptr(mmap(nullptr, size, kDefaultRights, kFlags, -1, 0)) {
  if (ptr == MAP_FAILED) {
    perror("failed to allocate memory block of specified size");
    ptr = nullptr;
  }
}

flag_type Memory::RightsConvert(Memory::Rights rights) noexcept {
  switch (rights) {
    case kRead:
      return PROT_READ;
    case kWrite:
      return PROT_WRITE;
    case kExecute:
      return PROT_EXEC;
    default:
      return PROT_NONE;
  }
}

void Memory::SetRights(const std::vector<Memory::Rights>& rights) {
  auto rights_conv = RightsConvert(rights);
  if (mprotect(ptr, size, rights_conv) < 0) {
    perror("failed to assign new rights");
    throw std::exception();
  }
}

flag_type
Memory::RightsConvert(const std::vector<Memory::Rights>& rights) noexcept {
  flag_type prot = 0;
  for (Memory::Rights right : rights) {
    prot |= RightsConvert(right);
  }
  return prot;
}

void Memory::FillWithBytes(const std::vector<byte_type>& bytes) {
  SetRights({Rights::kWrite});
  std::copy(bytes.begin(), bytes.end(), (byte_type*) ptr);
  SetRights({Rights::kRead});
}

Memory::~Memory() {
  if (munmap(ptr, size) < 0) {
    perror("failed to munmap pointer");
  }
}