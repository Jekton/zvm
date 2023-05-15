#pragma once

#include <stdarg.h>
#include <stdio.h>

#include "zvm_config.h"

#define ensure(x, msg, ...) do {                 \
    if (!(x)) {                             \
        fprintf(stderr, "[%s:%d] " msg "\n", \
                __FILE__, __LINE__, ##__VA_ARGS__);      \
        exit(1);                            \
    }                                       \
} while (0)

#define check(x) do {                       \
    if (!(x)) {                             \
        fprintf(stderr, "[%s:%d] check failed: %s\n", \
                __FILE__, __LINE__, #x);      \
        exit(1);                            \
    }                                       \
} while (0)

#if DEBUG

#define LOGD(tag, msg, ...) {               \
    printf("[DEBUG][%s] %s: " msg "\n",            \
           tag, __func__, ##__VA_ARGS__);   \
} while (0)

#else

#define LOGD(tag, msg, ...) do { \
} while (0)

#endif

#define LOGW(tag, msg, ...) {               \
    printf("[WARN][%s] %s: " msg "\n",      \
           tag, __func__, ##__VA_ARGS__);   \
} while (0)


#define Nullable
#define Nonnull
