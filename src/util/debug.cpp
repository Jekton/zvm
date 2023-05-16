
#include "util/debug.h"

#include <atomic>
#include <memory>
#include <mutex>

namespace zvm::util {

static std::mutex sMutex;

FILE* getLogFile() {
    static std::atomic<FILE*> logFile(nullptr);
    auto file = logFile.load(std::memory_order::memory_order_acquire);
    if (file) return file;

    std::lock_guard<std::mutex> lock(sMutex);
    file = logFile.load(std::memory_order::memory_order_acquire);
    if (file) return file;
    file = fopen("zvm.log", "w");
    if (!file) file = stderr;
    logFile.store(file, std::memory_order::memory_order_release);
    return file;
}

}