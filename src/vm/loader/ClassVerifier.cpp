
#include "loader/ClassVerifier.h"

namespace zvm::loader {

constexpr u4 kMagic = 0xcafebabe;

bool ClassVerifier::verify() const {
    if (mClassFile->magic() != kMagic) {
        return false;
    }
    return true;
}


}