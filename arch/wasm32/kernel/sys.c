/* #include <linux/unistd.h> */
#include <asm/syscall.h>
#include <asm/linkage.h> /* __wasm_export */
#include <asm/ar.h>
#include <asm/syscall_wrapper.h>
#include <linux/printk.h>

#define AR_FB(ar, ...) AR_FB##ar(__VA_ARGS__)
#define AR_FB0()
#define AR_FB1(t, n) , (unsigned long)n
#define AR_FB2(t, n, ...) , (unsigned long)n AR_FB1(__VA_ARGS__)
#define AR_FB3(t, n, ...) , (unsigned long)n AR_FB2(__VA_ARGS__)
#define AR_FB4(t, n, ...) , (unsigned long)n AR_FB3(__VA_ARGS__)
#define AR_FB5(t, n, ...) , (unsigned long)n AR_FB4(__VA_ARGS__)
#define AR_FB6(t, n, ...) , (unsigned long)n AR_FB5(__VA_ARGS__)

#define AR_LF0
#define AR_LF1 "%lu"
#define AR_LF2 AR_LF1 "," AR_LF1
#define AR_LF3 AR_LF2 "," AR_LF1
#define AR_LF4 AR_LF3 "," AR_LF1
#define AR_LF5 AR_LF4 "," AR_LF1
#define AR_LF6 AR_LF5 "," AR_LF1

#define SYSCALL_DEBUG(ar, name, ret, ...) \
	METAL_SYSCALL_DEFINE(ar, name, __VA_ARGS__) { \
		pr_err(#name "(d)[%d] ("AR_LF##ar")\n", \
		       ar AR_FB(ar, __VA_ARGS__)); \
		return ret; \
	}

#define SYSCALL_LDEBUG(ar, name, ret) SYSCALL_DEBUG(ar, name, ret, AR_LGEN##ar)

#define METAL_SYSCALL_DECLARE(ar, name, ...) \
        long sys_m_##name(__VA_ARGS__);

#include <uapi/linux/uio.h>

METAL_SYSCALL_DECLARE(3, writev, int, const struct iovec *, int);
METAL_SYSCALL_DECLARE(3, read, int, void*, size_t);

/* SYSCALL_DEBUG(1, exit, a, int, a); */
SYSCALL_DEBUG(3, ioctl, 0, int, fd, int, req, void *, arg);

typedef asmlinkage long (*sys_call_ptr_t)();

#undef __SYSCALL
#define __SYSCALL(nr, call) [nr] = (sys_call_ptr_t) (call),

asmlinkage const sys_call_ptr_t sys_call_table[__NR_syscalls+1] = {
	[ 0 ... __NR_syscalls ] = sys_ni_syscall,
#include <asm/unistd.h>
};

long sys_ni_syscall(void);

#define DEFSYSCALL(ar) \
	asmlinkage __visible __wasm_export long __init __syscall##ar(long n AR_DECL(ar, AR_LGEN##ar)) { \
		if (sys_call_table[n] == sys_ni_syscall) { \
			pr_err("ni_syscall[%d][%ld] ("AR_LF##ar")\n", \
				   ar, n AR_FB(ar, AR_LGEN##ar)); \
			return sys_ni_syscall(); \
		} \
		return sys_call_table[n](AR_PASS_(ar, AR_LGEN##ar)); \
	}

DEFSYSCALL(0);
DEFSYSCALL(1);
DEFSYSCALL(2);
DEFSYSCALL(3);
DEFSYSCALL(4);
DEFSYSCALL(5);
DEFSYSCALL(6);
