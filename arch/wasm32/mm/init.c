/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/memblock.h>
#include <linux/of_fdt.h>
#include <linux/swap.h>
#include <linux/module.h>
#include <linux/highmem.h>
#include <asm/page.h>
#include <asm/pgalloc.h>
#include <asm/sections.h>
#include <asm/arcregs.h>

pgd_t swapper_pg_dir[PTRS_PER_PGD] __aligned(PAGE_SIZE);
char empty_zero_page[PAGE_SIZE] __aligned(PAGE_SIZE);
EXPORT_SYMBOL(empty_zero_page);

static const unsigned long low_mem_start = CONFIG_LINUX_RAM_BASE;
static unsigned long low_mem_sz;

long __init arc_get_mem_sz(void)
{
	return low_mem_sz;
}

/* User can over-ride above with "mem=nnn[KkMm]" in cmdline */
static int __init setup_mem_sz(char *str)
{
	low_mem_sz = memparse(str, NULL) & PAGE_MASK;

	/* early console might not be setup yet - it will show up later */
	pr_info("\"mem=%s\": mem sz set to %ldM\n", str, TO_MB(low_mem_sz));

	return 0;
}
early_param("mem", setup_mem_sz);

void __init early_init_dt_add_memory_arch(u64 base, u64 size)
{
	int in_use = 0;

	if (!low_mem_sz) {
		if (base != low_mem_start)
			panic("CONFIG_LINUX_RAM_BASE != DT memory { }");

		low_mem_sz = size;
		in_use = 1;
	}

	pr_info("Memory @ %llx [%lldM] %s\n",
		base, TO_MB(size), !in_use ? "Not used":"");
}

/*
 * First memory setup routine called from setup_arch()
 * 1. setup swapper's mm @init_mm
 * 2. Count the pages we have and setup bootmem allocator
 * 3. zone setup
 */

#include <asm/sections.h>

void __init setup_arch_memory(void)
{
	unsigned long zones_size[MAX_NR_ZONES];
	unsigned long zones_holes[MAX_NR_ZONES];

	init_mm.start_code = (unsigned long)_text;
	init_mm.end_code = (unsigned long)_etext;
	init_mm.end_data = (unsigned long)_edata;
	init_mm.brk = (unsigned long)_end;

	pr_err("'setup_arch_memory' %s: Implement me properly\n"
	       "start_code: %lu\nend_code: %lu\nend_data: %lu\nbrk: %lu\n", __func__,
	       init_mm.start_code,
	       init_mm.end_code,
	       init_mm.end_data,
	       init_mm.brk);

	/* first page of system - kernel .vector starts here */
	min_low_pfn = ARCH_PFN_OFFSET;

	/* Last usable page of low mem */
	max_low_pfn = max_pfn = PFN_DOWN(low_mem_start + low_mem_sz);

#ifdef CONFIG_FLATMEM
	/* pfn_valid() uses this */
	max_mapnr = max_low_pfn - min_low_pfn;
#endif

	/*------------- bootmem allocator setup -----------------------*/

	/*
	 * seed the bootmem allocator after any DT memory node parsing or
	 * "mem=xxx" cmdline overrides have potentially updated @arc_mem_sz
	 *
	 * Only low mem is added, otherwise we have crashes when allocating
	 * mem_map[] itself. NO_BOOTMEM allocates mem_map[] at the end of
	 * avail memory, ending in highmem with a > 32-bit address. However
	 * it then tries to memset it with a truncaed 32-bit handle, causing
	 * the crash
	 */

	memblock_add_node(low_mem_start, low_mem_sz, 0);
	memblock_reserve(low_mem_start, __pa(_end) - low_mem_start);

	early_init_fdt_reserve_self();
	early_init_fdt_scan_reserved_mem();

	memblock_dump_all();

	/*----------------- node/zones setup --------------------------*/
	memset(zones_size, 0, sizeof(zones_size));
	memset(zones_holes, 0, sizeof(zones_holes));

	zones_size[ZONE_NORMAL] = max_low_pfn - min_low_pfn;
	zones_holes[ZONE_NORMAL] = 0;

	/*
	 * We can't use the helper free_area_init(zones[]) because it uses
	 * PAGE_OFFSET to compute the @min_low_pfn which would be wrong
	 * when our kernel doesn't start at PAGE_OFFSET, i.e.
	 * PAGE_OFFSET != CONFIG_LINUX_RAM_BASE
	 */
	free_area_init_node(0,			/* node-id */
			    zones_size,		/* num pages per zone */
			    min_low_pfn,	/* first pfn of node */
			    zones_holes);	/* holes */
}

/*
 * mem_init - initializes memory
 *
 * Frees up bootmem
 * Calculates and displays memory available/used
 */
void __init mem_init(void)
{
	pr_err("mem_init %s.\n", __func__);
	memblock_free_all();
	mem_init_print_info(NULL);
}

/*
 * free_initmem: Free all the __init memory.
 */
void __ref free_initmem(void)
{
	free_initmem_default(-1);
}
