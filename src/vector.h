#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <assert.h>

#define SWAP(x, y, T)                           \
    do {                                        \
        T temp = x;                             \
        x = y;                                  \
        y = temp;                               \
    } while(0)

#define _VECTOR_STRUCT(type)                          \
    typedef struct{                                   \
        type* array;                                  \
        size_t size;                                  \
        size_t cap;                                   \
    } type##Vec;

#define _VECTOR_CREATE(type, capacity)                                  \
    type##Vec create_##type##Vec(){                                     \
        assert(capacity > 0);                                           \
        type* array = malloc(sizeof(type) * capacity);                  \
        assert(array != NULL);                                          \
        type##Vec vec = {.array = array, .size = 0, .cap = capacity};   \
        return vec;                                                     \
    }

#define _VECTOR_INSERT(type)                                            \
    void insert_##type##Vec(type##Vec* vec, type el){                   \
    if (vec->size == vec->cap){                                         \
        vec->cap *= 2;                                                  \
        vec->array = realloc(vec->array, sizeof(type) * vec->cap);      \
        assert (vec->array != NULL);                                    \
    }                                                                   \
    vec->array[vec->size++] = el;                                       \
}

#define _VECTOR_POP(type)                       \
    type pop_##type##Vec(type##Vec* vec){       \
        assert(vec->size != 0);                 \
        return vec->array[--vec->size];         \
    }

#define VECTOR_IMPLEMENTATION(type, capacity)            \
    _VECTOR_STRUCT(type)                                 \
         _VECTOR_CREATE(type, capacity)                  \
         _VECTOR_INSERT(type)                            \
         _VECTOR_POP (type)

#define QUICKSORT_IMPLEMENTATION(type, compare_func)                \
    ssize_t _partition_##type##Vec(type* arr, ssize_t l, ssize_t r) {  \
        size_t i = l + 1;                                           \
        size_t j = r;                                               \
        type m = arr[l];                                          \
        do {                                                        \
            while((compare_func(arr[i], m) <= 0) && i < j) {        \
                i++;                                                \
            }                                                       \
            while((compare_func(arr[j], m) >= 0) && j > i) {        \
                j--;                                                \
            }                                                       \
            if (i < j){                                             \
                SWAP(arr[i], arr[j], type);                         \
            }                                                       \
        } while (i < j);                                            \
        if (compare_func(arr[i], m) > 0){                           \
            SWAP(arr[i-1], arr[l], type);                           \
            return i - 1;                                           \
        } else {                                                    \
            SWAP(arr[i], arr[l], type);                             \
            return i;                                               \
        }                                                           \
    }                                                               \
                                                                    \
    void _quicksort_##type##Vec(type* arr, ssize_t l, ssize_t r){     \
        if(l >= r){                                                 \
            return;                                                 \
        }                                                           \
        ssize_t m = _partition_##type##Vec(arr, l, r);               \
        _quicksort_##type##Vec(arr, l, m - 1);                      \
        _quicksort_##type##Vec(arr, m + 1, r);                      \
    }                                                               \
                                                                    \
    void quicksort_##type##Vec(type##Vec* vec){                     \
        _quicksort_##type##Vec(vec->array, 0, vec->size - 1);       \
    }

#endif // VECTOR_HEADER
