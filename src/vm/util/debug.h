#pragma once

#include <stdarg.h>
#include <stdio.h>

#include "zvm_config.h"

namespace zvm::util {

FILE* getLogFile();

}


#define ensure(x, msg, ...) do {                    \
    if (!(x)) {                                     \
        fprintf(zvm::util::getLogFile(),            \
                "[%s:%d] " msg "\n",                \
                __FILE__, __LINE__, ##__VA_ARGS__); \
        exit(1);                                    \
    }                                               \
} while (0)

#define check(x) do {                           \
    if (!(x)) {                                 \
        fprintf(zvm::util::getLogFile(),        \
                "[%s:%d] check failed: %s\n",   \
                __FILE__, __LINE__, #x);        \
        exit(1);                                \
    }                                           \
} while (0)

#if DEBUG

#define LOGD(tag, msg, ...) {               \
    fprintf(zvm::util::getLogFile(),        \
            "[DEBUG][%s] %s: " msg "\n",    \
            tag, __func__, ##__VA_ARGS__);  \
} while (0)

#else

#define LOGD(tag, msg, ...) do { \
} while (0)

#endif

#define LOGW(tag, msg, ...) {               \
    fprintf(zvm::util::getLogFile(),        \
            "[WARN][%s] %s: " msg "\n",     \
            tag, __func__, ##__VA_ARGS__);  \
} while (0)


#define Nullable
#define Nonnull
