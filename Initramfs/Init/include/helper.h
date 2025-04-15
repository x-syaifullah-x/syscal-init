#ifndef HELPER_H
#define HELPER_H

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

typedef struct {
    const char *path;
    const char *value;
} w_data;

typedef struct {
    const char *upper;
    const char *work;
    const char *merge;
    const char *data;
    int mode;
} s_overlay;

inline long long_to_str(long long v, char *b) {
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

inline long strlen(const char *s) {
    long i = 0;
    while (s[i]) i++;
    return i;
}

inline char *strcpy(char *dst, const char *src) {
    char *ret = dst;
    while ((*dst++ = *src++));
    return ret;
}

inline char *strcat(char *dst, const char *src) {
    char *ret = dst;
    while (*dst) dst++;
    while ((*dst++ = *src++));
    return ret;
}

#endif