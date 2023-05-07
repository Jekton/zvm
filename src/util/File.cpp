
#include "util/File.h"

namespace zvm::util {


File openFile(const char* path, const char* mode) {
    auto file = fopen(path, mode);
    return File(file, &fclose);
}

}