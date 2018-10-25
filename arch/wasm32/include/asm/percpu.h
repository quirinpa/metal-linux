#ifndef ARCH_WASM_PERCPU_H
#define ARCH_WASM_PERCPU_H
#include <asm-generic/percpu.h>
#include <linux/printk.h>

static void pcpu_free_vm_areas(void* data, size_t len) {
	pr_info("pcpu_free_vm_areas");
}

static struct vm_struct ** pcpu_get_vm_areas(const unsigned long * group_offsets, const unsigned int * group_sizes, char nr_groups, char atom_size) {
	pr_info("pcpu_get_vm_areas");
	return NULL;

}
#endif
