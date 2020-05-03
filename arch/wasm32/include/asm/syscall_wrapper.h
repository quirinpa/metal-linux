#ifndef ARCH_WASM_SYSCALL_WRAPPER_H
#define ARCH_WASM_SYSCALL_WRAPPER_H

#include <asm/ar.h>

long sys_ni_syscall(void);

#define METAL_SYSCALL_DEFINE(ar, name, ...) \
	long sys_##name##_##ar(AR_DECL_(ar, __VA_ARGS__)); \
	long sys_m_##name(AR_DECL_(ar, __VA_ARGS__)) { \
		return sys_##name##_##ar(AR_MCAST_(ar, __VA_ARGS__)); \
	} \
        long sys_##name##_##ar(AR_DECL_(ar, __VA_ARGS__))

#endif /* ARCH_WASM_SYSCALL_WRAPPER_H */
