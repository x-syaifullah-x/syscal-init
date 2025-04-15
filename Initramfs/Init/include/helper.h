#ifndef HELPER_H
#define HELPER_H

#include <stddef.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

inline size_t strlen(const char *__s) {
    size_t i = 0;
    while (__s[i]) i++;
    return i;
}

inline char *strcpy(char *__restrict__ __dest, const char *__restrict__ __src) {
    char *ret = __dest;
    while ((*__dest++ = *__src++));
    return ret;
}

inline char *strcat(char *__restrict__ __dest, const char *__restrict__ __src) {
    char *ret = __dest;
    while (*__dest) __dest++;
    while ((*__dest++ = *__src++));
    return ret;
}

inline long strtol(const char *s) {
    long n = 0;
    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s++ - '0');
        if (n < 0) return 0;
    }
    return n;
}

inline size_t ltoa(long long v, char *b) {
    char *p = b;
    long long neg = v < 0;
    if (neg) v = -v;
    char *s = p;
    do {
        *p++ = '0' + (v % 10);
        v /= 10;
    } while (v);
    if (neg) *p++ = '-';
    for (char *e = p - 1; s < e; s++, e--) {
        char tmp = *s;
        *s = *e;
        *e = tmp;
    }
    return p - b;
}

#endif  // HELPER_H