/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef ARCH_WASM_PAGE_H
#define ARCH_WASM_PAGE_H

/* #define CONFIG_FLATMEM */
#define NR_PAGEFLAGS __NR_PAGEFLAGS

#define PAGE_SHIFT 16 // 64K
#include <linux/const.h>
#define PAGE_SIZE	_BITUL(PAGE_SHIFT)	/* Default 8K */
#define PAGE_OFFSET	_AC(0x80000000, UL)	/* Kernel starts at 2G onwrds */
#define PAGE_MASK	(~(PAGE_SIZE-1))

#ifndef __ASSEMBLY__

#define clear_page(paddr)		memset((paddr), 0, PAGE_SIZE)
#define copy_page(to, from)		memcpy((to), (from), PAGE_SIZE)

struct vm_area_struct;
struct page;

#define __HAVE_ARCH_COPY_USER_HIGHPAGE

void copy_user_highpage(struct page *to, struct page *from,
			unsigned long u_vaddr, struct vm_area_struct *vma);
void clear_user_page(void *to, unsigned long u_vaddr, struct page *page);
/* #define copy_user_page(to, from, vaddr, pg) copy_page(to, from) */

#undef STRICT_MM_TYPECHECKS

#ifdef STRICT_MM_TYPECHECKS

typedef struct {
	unsigned long pte;
} pte_t;
typedef struct {
	unsigned long pgd;
} pgd_t;
typedef struct {
	unsigned long pgprot;
} pgprot_t;

#define pte_val(x)      ((x).pte)
#define pgd_val(x)      ((x).pgd)
#define pgprot_val(x)   ((x).pgprot)

#define __pte(x)        ((pte_t) { (x) })
#define __pgd(x)        ((pgd_t) { (x) })
#define __pgprot(x)     ((pgprot_t) { (x) })

#define pte_pgprot(x) __pgprot(pte_val(x))

#else /* !STRICT_MM_TYPECHECKS */

typedef unsigned long pte_t;
typedef unsigned long pgd_t;
typedef unsigned long pgprot_t;

#define pte_val(x)	(x)
#define pgd_val(x)	(x)
#define pgprot_val(x)	(x)
#define __pte(x)	(x)
#define __pgd(x)	(x)
#define __pgprot(x)	(x)
#define pte_pgprot(x)	(x)

#endif

typedef pte_t * pgtable_t;

/*
 * Use virt_to_pfn with caution:
 * If used in pte or paddr related macros, it could cause truncation
 * in PAE40 builds
 * As a rule of thumb, only use it in helpers starting with virt_
 * You have been warned !
 */
#define virt_to_pfn(kaddr)	(__pa(kaddr) >> PAGE_SHIFT)

#define ARCH_PFN_OFFSET		virt_to_pfn(CONFIG_LINUX_RAM_BASE)

#ifdef CONFIG_FLATMEM
#define pfn_valid(pfn)		(((pfn) - ARCH_PFN_OFFSET) < max_mapnr)
#endif

/*
 * __pa, __va, virt_to_page (ALERT: deprecated, don't use them)
 *
 * These macros have historically been misnamed
 * virt here means link-address/program-address as embedded in object code.
 * And for ARC, link-addr = physical address
 */
#define __pa(vaddr)  ((unsigned long)(vaddr))
#define __va(paddr)  ((void *)((unsigned long)(paddr)))

#define virt_to_page(kaddr)	pfn_to_page(virt_to_pfn(kaddr))
#define virt_addr_valid(kaddr)  pfn_valid(virt_to_pfn(kaddr))

/* Default Permissions for stack/heaps pages (Non Executable) */
#define VM_DATA_DEFAULT_FLAGS   (VM_READ | VM_WRITE | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

#define WANT_PAGE_VIRTUAL   1

#include <asm-generic/memory_model.h>   /* page_to_pfn, pfn_to_page */
#include <asm-generic/getorder.h>

#endif /* !__ASSEMBLY__ */

#endif
