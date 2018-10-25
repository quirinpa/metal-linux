#ifndef ARCH_WASM_STRING_H
#define ARCH_WASM_STRING_H

#define __HAVE_ARCH_MEMMOVE
void *memmove(void *v_dst, const void *v_src, __kernel_size_t c);
#define __HAVE_ARCH_MEMZERO
void memzero(void *ptr, __kernel_size_t n);
#include <asm-generic/string.h>
#endif
