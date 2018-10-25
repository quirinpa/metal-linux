/* SPDX-License-Identifier: GPL-2.0 */
/* #ifndef _ASM_X86_TIMEX_H */
#ifndef _ASM_WASM_TIMEX_H
#define _ASM_WASM_TIMEX_H

/* #include <asm/processor.h> */
/* #include <asm/tsc.h> */
/* #include <linux/time.h> */
/* #include <linux/time32.h> */

/* #include <linux/seqlock.h> */

/* Assume we use the PIT time source for the clock tick */
#define CLOCK_TICK_RATE		80000000
#include <asm-generic/timex.h>

/* #define ARCH_HAS_READ_CURRENT_TIMER */

#endif /* _ASM_WASM_TIMEX_H */
