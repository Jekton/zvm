
#pragma once

#include <stdio.h>
#include <memory>

#include "util/types.h"
namespace zvm::util {

class File {
public:
    File(const char* path, const char* mode)
        : mFile(fopen(path, mode)) { }

    ~File() {
        if (mFile) fclose(mFile);
    }

    File(const File&) = delete;
    File& operator=(const File&) = delete;

    File(File&& other);
    File& operator=(File&& other);

    void swap(File&& other);

    bool isValid() const { return mFile != nullptr; }

    template<typename T>
    bool read(T *ptr);

    template<typename T>
    bool read(T *ptr, size_t nitems);

    bool readBigEndian(u2* ptr);
    bool readBigEndian(u4* ptr);
    bool readBigEndian(u8* ptr);

private:
    File() { };

    FILE* mFile = nullptr;
};

template<typename T>
bool File::read(T *ptr) {
    return read(ptr, 1);
}

template<typename T>
bool File::read(T *ptr, size_t nitems) {
    return fread(ptr, sizeof(T), nitems, mFile) == nitems;
}

}