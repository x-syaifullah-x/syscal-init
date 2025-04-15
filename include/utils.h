#ifndef UTILS_H
#define UTILS_H

static inline long strlen(const char *s) {
    long i = 0;
    while (s[i]) i++;
    return i;
}

static inline long long_to_str(long v, char *b) {
    char *p = b;
    int neg = v < 0;
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

#endif