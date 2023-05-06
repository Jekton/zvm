
#include <stdio.h>

#include "zvm_config.h"

int main(int argc, char* argv[]) {
    printf("Hello, zvm %d.%d.%d!\n", kZvmVersionMajor, kZvmVersionMinor, kZvmVersionPatch);
    return 0;
}