/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_WASM_TYPES_H
#define __ASM_WASM_TYPES_H

#include <uapi/asm/types.h>

typedef __u8 u8;
typedef __u16 u16;
typedef __u32 u32;
typedef __u64 u64;

typedef __s8 s8;
typedef __s16 s16;
typedef __s32 s32;
typedef __s64 s64;

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
#ifndef __ASSEMBLY__

typedef u16 insn_size_t;
typedef u32 reg_size_t;

#endif /* __ASSEMBLY__ */
#endif /* __ASM_WASM_TYPES_H */
