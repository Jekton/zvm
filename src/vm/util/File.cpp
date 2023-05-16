
#include "util/File.h"

#include <arpa/inet.h>

namespace zvm::util {

File::File(File&& other) {
    mFile = other.mFile;
    other.mFile = nullptr;
}

File& File::operator=(File&& other) {
    swap(std::move(other));
    return *this;
}

void File::swap(File&& other) {
    std::swap(mFile, other.mFile);
}

bool File::readBigEndian(u2 *ptr) {
    u2 x;
    if (fread(&x, 2, 1, mFile) != 1) {
        return false;
    }
    *ptr = ntohs(x);
    return true;
}

bool File::readBigEndian(u4 *ptr) {
    u4 x;
    if (fread(&x, 4, 1, mFile) != 1) {
        return false;
    }
    *ptr = ntohl(x);
    return true;
}

bool File::readBigEndian(u8 *ptr) {
    u8 x;
    if (fread(&x, 8, 1, mFile) != 1) {
        return false;
    }
    *ptr = ntohll(x);
    return true;
}

}