#ifndef ARCH_WASM_IO_H
#define ARCH_WASM_IO_H

/* TODO is this supposed to happen? */

/* ssize_t js_emem(size_t, u8 *, size_t); */

/* static ssize_t */
/* emem(void * data, size_t len) */
/* { */
/* 	/1* static unsigned const end = 2048; *1/ */
/* 	static unsigned const start = 4; */
/* 	static unsigned ofs = start; */
/*         ssize_t r = js_emem(ofs, (u8*) data, len); */
/*         ofs += r; */
/*         return r; */
/* } */

/* #define __raw_readb __raw_readb */
/* static inline u8 __raw_readb(const volatile void __iomem *addr) { */
/*         u8 ret; */
/*         emem(&ret, sizeof(ret)); */
/*         return ret; */
/* } */

/* #define __raw_readw __raw_readw */
/* static inline u16 __raw_readw(const volatile void __iomem *addr) { */
/*         u16 ret; */
/*         emem(&ret, sizeof(ret)); */
/*         return ret; */
/* } */

/* #define __raw_readl __raw_readl */
/* static inline u32 __raw_readl(const volatile void __iomem *addr) { */
/*         u32 ret; */
/*         emem(&ret, sizeof(ret)); */
/*         return ret; */
/* } */

/* #define __raw_readq __raw_readq */
/* static inline u64 __raw_readq(const volatile void __iomem *addr) { */
/*         u64 ret; */
/*         emem(&ret, sizeof(ret)); */
/*         return ret; */
/* } */

#include <asm-generic/io.h>
#include <asm/page.h>

#define page_to_phys(page) (page_to_pfn(page) << PAGE_SHIFT)

#endif
