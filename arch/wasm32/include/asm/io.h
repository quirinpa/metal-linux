#ifndef ARCH_WASM_IO_H
#define ARCH_WASM_IO_H

#include <asm-generic/io.h>
#include <asm/page.h>

#define page_to_phys(page) (page_to_pfn(page) << PAGE_SHIFT)

#endif
