#pragma once

#include <stdarg.h>
#include <stdio.h>

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