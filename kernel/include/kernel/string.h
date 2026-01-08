/* SYNAPSE SO - String Library Header */
/* Licensed under GPLv3 */

#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

/* Get string length */
int strlen(const char* str);

/* Compare two strings */
int strcmp(const char* s1, const char* s2);

/* Copy string */
char* strcpy(char* dest, const char* src);

/* Copy string with size limit */
char* strncpy(char* dest, const char* src, int n);

/* Compare strings with size limit */
int strncmp(const char* s1, const char* s2, int n);

/* Copy memory */
void* memcpy(void* dest, const void* src, unsigned int n);

/* Set memory */
void* memset(void* s, int c, unsigned int n);

#endif /* KERNEL_STRING_H */
