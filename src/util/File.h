
#pragma once

#include <stdio.h>
#include <memory>

namespace zvm::util {

using File = std::unique_ptr<FILE, decltype(&fclose)>;

File openFile(const char* path, const char* mode);

}