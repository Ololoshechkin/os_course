//
// Created by Vadim on 24/05/2018.
//

#include <sys/errno.h>
#include <unistd.h>
#include "ExceptionHelp.h"

std::string GetErrorMessage(const std::string& description) {
  const auto system_msg = strerror(errno);
//  perror(description.c_str());
  return description + " (" + system_msg + ")";
}