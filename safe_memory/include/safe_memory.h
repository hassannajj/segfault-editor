#ifndef SAFE_MEMORY_H
#define SAFE_MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Safe malloc wrapper
void *safe_malloc(size_t size);

// Safe realloc wrapper
void *safe_realloc(void *ptr, size_t size);

#endif // SAFE_MEMORY_H

