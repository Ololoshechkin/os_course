//
// Created by Vadim on 24/05/2018.
//

#include <sys/errno.h>
#include <cstring>
#include <unistd.h>
#include "ExceptionHelp.h"

std::string GetErrorMessage(const std::string& description) {
  const auto system_msg = std::strerror(errno);
//  perror(description.c_str());
  return description + " (" + system_msg + ")";
}
