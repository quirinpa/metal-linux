#ifndef __ASM_ARC_IRQFLAGS_H
#define __ASM_ARC_IRQFLAGS_H

static unsigned long wasm_irq_flags;

static inline void arch_local_irq_restore(unsigned long flags) {
	wasm_irq_flags = flags;
}
static unsigned long arch_local_save_flags(void) {
	return wasm_irq_flags;
}

#include <asm-generic/irqflags.h>
#endif // __ASM_ARC_IRQFLAGS_H
