/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_WASM_LINKAGE_H
#define _ASM_WASM_LINKAGE_H

#include <linux/stringify.h>

#undef notrace
#define notrace __attribute__((no_instrument_function))

/* #ifdef CONFIG_WASM_32 */
/* #define asmlinkage CPP_ASMLINKAGE __attribute__((regparm(0))) */
/* #undef asmlinkage */
/* #define asmlinkage */
/* #endif /1* CONFIG_WASM_32 *1/ */

#ifdef __ASSEMBLY__

#define GLOBAL(name)	\
	.globl name;	\
	name:

#if defined(CONFIG_WASM_64) || defined(CONFIG_WASM_ALIGNMENT_16)
#define __ALIGN		.p2align 4, 0x90
#define __ALIGN_STR	__stringify(__ALIGN)
#endif

#endif /* __ASSEMBLY__ */

// not working, needs explicit export
#define __wasm_export __attribute__((used)) __attribute__((visibility("default")))

#define cond_syscall(x)	long x() __attribute__((weak, alias("sys_ni_syscall")))

#endif /* _ASM_WASM_LINKAGE_H */

