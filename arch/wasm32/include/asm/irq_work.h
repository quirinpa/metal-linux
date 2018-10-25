/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_WASM_IRQ_WORK_H
#define __ASM_WASM_IRQ_WORK_H

static inline bool arch_irq_work_has_interrupt(void)
{
	return false;
}

#endif /* _ASM_WASM_IRQ_WORK_H */
