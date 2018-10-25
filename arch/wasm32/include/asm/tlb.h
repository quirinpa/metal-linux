/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _ASM_WASM_TLB_H
#define _ASM_WASM_TLB_H

#define tlb_flush(tlb)				\
do {						\
	if (tlb->fullmm)			\
		flush_tlb_mm((tlb)->mm);	\
} while (0)

/*
 * This pair is called at time of munmap/exit to flush cache and TLB entries
 * for mappings being torn down.
 * 1) cache-flush part -implemented via tlb_start_vma( ) for VIPT aliasing D$
 * 2) tlb-flush part - implemted via tlb_end_vma( ) flushes the TLB range
 *
 * Note, read http://lkml.org/lkml/2004/1/15/6
 */
#ifndef CONFIG_ARC_CACHE_VIPT_ALIASING
#define tlb_start_vma(tlb, vma)
#else
#define tlb_start_vma(tlb, vma)						\
do {									\
	if (!tlb->fullmm)						\
		flush_cache_range(vma, vma->vm_start, vma->vm_end);	\
} while(0)
#endif

#define tlb_end_vma(tlb, vma)						\
do {									\
	if (!tlb->fullmm)						\
		flush_tlb_range(vma, vma->vm_start, vma->vm_end);	\
} while (0)

#define __tlb_remove_tlb_entry(tlb, ptep, address)

#include <linux/pagemap.h>
#include <asm-generic/tlb.h>

#define __SCHED_FEAT_HRTICK 0
#define __SCHED_FEAT_RT_RUNTIME_SHARE 0
#define __SCHED_FEAT_START_DEBIT 0
#define __SCHED_FEAT_GENTLE_FAIR_SLEEPERS 0
#define __SCHED_FEAT_NEXT_BUDDY 0
#define __SCHED_FEAT_LAST_BUDDY 0
#define __SCHED_FEAT_WAKEUP_PREEMPTION 0
#define __SCHED_FEAT_ATTACH_AGE_LOAD 0

#endif /* _ASM_WASM_TLB_H */
