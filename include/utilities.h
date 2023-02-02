#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>

#ifdef NDEBUG

#define CHECK(_pointer)         \
    do {                        \
        if (!(_pointer))        \
        {                       \
            exit(EXIT_FAILURE); \
        }                       \
    } while (0)

#else

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define CHECK(_pointer)                                                  \
    do {                                                                 \
        if (!(_pointer))                                                 \
        {                                                                \
            fprintf(stderr, "errno: %d\n%s\n", errno, strerror(errno));  \
            exit(EXIT_FAILURE);                                          \
        }                                                                \
    } while (0)

#endif

#define SILENCE_UNUSED(x)   \
    (void)(x)

#endif
