#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#ifndef DYNAMIC_ARRAY_malloc
#define DYNAMIC_ARRAY_malloc malloc
#endif

#ifndef DYNAMIC_ARRAY_realloc
#define DYNAMIC_ARRAY_realloc realloc
#endif

#ifndef DYNAMIC_ARRAY_free
#define DYNAMIC_ARRAY_free free
#endif

#ifndef DYNAMIC_ARRAY_GROWTH_FACTOR
#define DYNAMIC_ARRAY_GROWTH_FACTOR 1.5
#endif

#ifndef DYNAMIC_ARRAY_INITIAL_CAPACITY
#define DYNAMIC_ARRAY_INITIAL_CAPACITY 2
#endif

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "utilities.h"

#define DYNAMIC_ARRAY_SIZE(dynamic_array)   \
    ((dynamic_array) ? ((size_t *)(dynamic_array))[-2] : (size_t)0)

#define DYNAMIC_ARRAY_CAPACITY(dynamic_array)   \
    ((dynamic_array) ? ((size_t *)(dynamic_array))[-1] : (size_t)0)

#define DYNAMIC_ARRAY_EMPTY(dynamic_array)  \
    (DYNAMIC_ARRAY_SIZE(dynamic_array) == 0)

#define DYNAMIC_ARRAY_PEEK(dynamic_array) \
    (dynamic_array)[DYNAMIC_ARRAY_SIZE(dynamic_array) - 1]

#define DYNAMIC_ARRAY_ALLOCATE(dynamic_array, requested_capacity)                                                               \
    do {                                                                                                                        \
        assert((requested_capacity) > 0);                                                                                       \
        const size_t _total_capacity = (requested_capacity) * sizeof(*(dynamic_array)) + (sizeof(size_t) * 2);                  \
        if (!(dynamic_array))                                                                                                   \
        {                                                                                                                       \
            const size_t * _dynamic_array_pointer = DYNAMIC_ARRAY_malloc(_total_capacity);                                      \
            CHECK(_dynamic_array_pointer);                                                                                      \
            (dynamic_array) = (void *)(&_dynamic_array_pointer[2]);                                                             \
            ((size_t *)(dynamic_array))[-1] = (requested_capacity);                                                             \
            ((size_t *)(dynamic_array))[-2] = 0;                                                                                \
        }                                                                                                                       \
        else                                                                                                                    \
        {                                                                                                                       \
            const size_t * _dynamic_array_pointer = DYNAMIC_ARRAY_realloc(&((size_t *)(dynamic_array))[-2], _total_capacity);   \
            CHECK(_dynamic_array_pointer);                                                                                      \
            (dynamic_array) = (void *)(&_dynamic_array_pointer[2]);                                                             \
            ((size_t *)(dynamic_array))[-1] = (requested_capacity);                                                             \
            if ((requested_capacity) < DYNAMIC_ARRAY_SIZE(dynamic_array))                                                       \
            {                                                                                                                   \
                ((size_t *)(dynamic_array))[-2] = (requested_capacity);                                                         \
            }                                                                                                                   \
        }                                                                                                                       \
    } while (0)

#define DYNAMIC_ARRAY_FREE(dynamic_array)       \
    do {                                        \
        assert(dynamic_array);                  \
        free(&((size_t *)(dynamic_array))[-2]); \
        (dynamic_array) = NULL;                 \
    } while (0)

#define DYNAMIC_ARRAY_RESERVE(dynamic_array, requested_capacity)            \
    do {                                                                    \
        assert((requested_capacity) > 0);                                   \
        if ((requested_capacity) > DYNAMIC_ARRAY_CAPACITY(dynamic_array))   \
        {                                                                   \
            DYNAMIC_ARRAY_ALLOCATE(dynamic_array, requested_capacity);      \
        }                                                                   \
    } while (0)

#define DYNAMIC_ARRAY_FIT(dynamic_array)                                                \
    do {                                                                                \
        assert(dynamic_array);                                                          \
        if(!DYNAMIC_ARRAY_EMPTY(dynamic_array))                                         \
        {                                                                               \
            DYNAMIC_ARRAY_ALLOCATE(dynamic_array, DYNAMIC_ARRAY_SIZE(dynamic_array));   \
        }                                                                               \
    } while (0)

#define DYNAMIC_ARRAY_PUSH_BACK(dynamic_array, value)                                                                                                   \
    do {                                                                                                                                                \
        const size_t _capacity_ = DYNAMIC_ARRAY_CAPACITY(dynamic_array), _size_ = DYNAMIC_ARRAY_SIZE(dynamic_array);                                    \
        if (_capacity_ == _size_)                                                                                                                       \
        {                                                                                                                                               \
            DYNAMIC_ARRAY_ALLOCATE((dynamic_array), !_capacity_ ? DYNAMIC_ARRAY_INITIAL_CAPACITY : (size_t)(_capacity_ * DYNAMIC_ARRAY_GROWTH_FACTOR)); \
        }                                                                                                                                               \
        (dynamic_array)[_size_] = (value);                                                                                                              \
        ((size_t *)(dynamic_array))[-2] = _size_ + 1;                                                                                                   \
    } while (0)

#define DYNAMIC_ARRAY_REMOVE_BACK(dynamic_array)                                                                                    \
    do {                                                                                                                            \
        assert((dynamic_array) && DYNAMIC_ARRAY_SIZE(dynamic_array) > 0);                                                           \
        ((size_t *)(dynamic_array))[-2] = ((size_t *)(dynamic_array))[-2] - 1;                                                      \
        if (DYNAMIC_ARRAY_SIZE(dynamic_array) == (size_t)(DYNAMIC_ARRAY_CAPACITY(dynamic_array) / DYNAMIC_ARRAY_GROWTH_FACTOR))     \
        {                                                                                                                           \
            DYNAMIC_ARRAY_FIT(dynamic_array);                                                                                       \
        }                                                                                                                           \
    } while (0)

#define DYNAMIC_ARRAY_POP_BACK(dynamic_array, destination)                  \
    do {                                                                    \
        assert((dynamic_array) && DYNAMIC_ARRAY_SIZE(dynamic_array) > 0);   \
        (destination) = DYNAMIC_ARRAY_PEEK(dynamic_array);                  \
        DYNAMIC_ARRAY_REMOVE_BACK(dynamic_array);                           \
    } while (0)

#define DYNAMIC_ARRAY_INSERT(dynamic_array, value, index)                                                       \
    do {                                                                                                        \
        const long _size = DYNAMIC_ARRAY_SIZE(dynamic_array), _move = (_size - 1 - (index));                    \
        assert((index) >= 0 && (index) <= _size);                                                               \
        if (_move)                                                                                              \
        {                                                                                                       \
            DYNAMIC_ARRAY_PUSH_BACK(dynamic_array, (dynamic_array)[_size - 1]);                                 \
            memmove(&(dynamic_array)[index + 1], &(dynamic_array)[index], _move * sizeof(*(dynamic_array)));    \
            (dynamic_array)[index] = (value);                                                                   \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            DYNAMIC_ARRAY_PUSH_BACK(dynamic_array, value);                                                      \
        }                                                                                                       \
    } while (0)

#define DYNAMIC_ARRAY_REMOVE(dynamic_array, index)                                                                          \
    do {                                                                                                                    \
        const size_t _size = DYNAMIC_ARRAY_SIZE(dynamic_array);                                                             \
        assert((dynamic_array) && (index) >= 0 && (index) <= _size - 1);                                                    \
        memmove(&(dynamic_array)[index], &(dynamic_array)[index + 1], (_size - 1 - (index)) * sizeof(*(dynamic_array)));    \
        DYNAMIC_ARRAY_REMOVE_BACK(dynamic_array);                                                                           \
    } while (0)

#define DYNAMIC_ARRAY_ERASE(dynamic_array, index, destination)                                                              \
    do {                                                                                                                    \
        assert((dynamic_array) && (index) >= 0 && (index) < DYNAMIC_ARRAY_SIZE(dynamic_array));                             \
        (destination) = (dynamic_array)[index];                                                                             \
        DYNAMIC_ARRAY_REMOVE(dynamic_array, index);                                                                         \
    } while (0)

#define DYNAMIC_ARRAY_TRUNCATE(dynamic_array, requested_size)                               \
    do {                                                                                    \
        assert((dynamic_array) && (requested_size) < DYNAMIC_ARRAY_SIZE(dynamic_array));    \
        ((size_t *)(dynamic_array))[-2] = (requested_size);                                 \
    } while (0)

#define DYNAMIC_ARRAY_EXTEND(dynamic_array, requested_size, value)      \
    do {                                                                \
        assert((requested_size) > DYNAMIC_ARRAY_SIZE(dynamic_array));   \
        while (DYNAMIC_ARRAY_SIZE(dynamic_array) < (requested_size))    \
        {                                                               \
            DYNAMIC_ARRAY_PUSH_BACK(dynamic_array, value);              \
        }                                                               \
    } while (0)

#define DYNAMIC_ARRAY_RESIZE(dynamic_array, requested_size, value)                              \
    do {                                                                                        \
        assert((requested_size) >= 0 && (requested_size) != DYNAMIC_ARRAY_SIZE(dynamic_array)); \
        if (DYNAMIC_ARRAY_SIZE(dynamic_array) > (requested_size))                               \
        {                                                                                       \
            DYNAMIC_ARRAY_TRUNCATE(dynamic_array, requested_size);                              \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            DYNAMIC_ARRAY_EXTEND(dynamic_array, requested_size, value);                         \
        }                                                                                       \
    } while (0)

#define DYNAMIC_ARRAY_CLEAR(dynamic_array)              \
    do {                                                \
        assert(!DYNAMIC_ARRAY_EMPTY(dynamic_array));    \
        ((size_t *)(dynamic_array))[-2] = 0;            \
    } while (0)

#define DYNAMIC_ARRAY_COPY(destination, source)                                                                                                 \
    do {                                                                                                                                        \
        assert(!DYNAMIC_ARRAY_EMPTY(source));                                                                                                   \
        DYNAMIC_ARRAY_ALLOCATE(destination, DYNAMIC_ARRAY_SIZE(source));                                                                        \
        memcpy(&((size_t *)(destination))[-2], &((size_t *)(source))[-2], DYNAMIC_ARRAY_SIZE(source) * sizeof(*(source)) + 2 * sizeof(size_t)); \
    } while (0)

#define DYNAMIC_ARRAY_CONCATENATE(destination, source)                                                                          \
    do {                                                                                                                        \
        assert(source);                                                                                                         \
        if(DYNAMIC_ARRAY_EMPTY(destination))                                                                                    \
        {                                                                                                                       \
            DYNAMIC_ARRAY_COPY(destination, source);                                                                            \
        }                                                                                                                       \
        else                                                                                                                    \
        {                                                                                                                       \
            DYNAMIC_ARRAY_ALLOCATE(destination, DYNAMIC_ARRAY_SIZE(destination) + DYNAMIC_ARRAY_SIZE(source));                  \
            memcpy(&(destination)[DYNAMIC_ARRAY_SIZE(destination)], source, DYNAMIC_ARRAY_SIZE(source) * sizeof(*(source)));    \
            ((size_t *)(destination))[-2] = DYNAMIC_ARRAY_SIZE(source) + DYNAMIC_ARRAY_SIZE(destination);                       \
        }                                                                                                                       \
    } while (0)

#endif
