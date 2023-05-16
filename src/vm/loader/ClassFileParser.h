#pragma once

#include <stdio.h>

#include <memory>

#include "loader/ClassFile.h"
#include "util/File.h"

namespace zvm::loader {

class ClassFileParser {
public:
    static std::unique_ptr<ClassFile> parse(util::File file);
};


}