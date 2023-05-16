
#include <stdio.h>

#include "dumper/ClassDumper.h"
#include "loader/ClassFileParser.h"
#include "util/File.h"
#include "zvm_config.h"

using namespace zvm;

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("Hello, zvm %d.%d.%d!\n", kZvmVersionMajor, kZvmVersionMinor, kZvmVersionPatch);
        return 0;
    }
    auto file = util::File("../test/HelloWorld.class", "r");
    auto classFile = loader::ClassFileParser::parse(std::move(file));
    dumper::ClassDumper dumper(std::move(classFile));
    dumper.dump(stdout);
    return 0;
}