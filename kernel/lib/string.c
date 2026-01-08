/* SYNAPSE SO - String Library */
/* Licensed under GPLv3 */

/* Get string length */
int strlen(const char* str) {
    int len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

/* Compare two strings */
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

/* Copy string */
char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while ((*d++ = *src++));
    return dest;
}

/* Copy string with size limit */
char* strncpy(char* dest, const char* src, int n) {
    int i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

/* Compare strings with size limit */
int strncmp(const char* s1, const char* s2, int n) {
    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) {
        return 0;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

/* Copy memory */
void* memcpy(void* dest, const void* src, unsigned int n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

/* Set memory */
void* memset(void* s, int c, unsigned int n) {
    unsigned char* p = (unsigned char*)s;

    while (n--) {
        *p++ = (unsigned char)c;
    }

    return s;
}
