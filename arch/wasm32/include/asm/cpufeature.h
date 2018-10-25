#ifndef ARCH_WASM_CPUFEATURE_H
#define ARCH_WASM_CPUFEATURE_H
static u8 boot_cpu_data = 0;

#define cpu_has(data, bit) (data & bit)

#define boot_cpu_has(bit) cpu_has(boot_cpu_data, bit)
#endif
