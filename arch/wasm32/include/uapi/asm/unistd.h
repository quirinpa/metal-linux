#if !defined(ARCH_WASM32_UNISTD_H) || defined(__SYSCALL)
#define ARCH_WASM32_UNISTD_H

#ifndef __SYSCALL
#define __SYSCALL(x, y)
#endif

/* kernel/exit.c */
/* #define __NR_exit 1 */
/* __SYSCALL(__NR_exit, sys_m_exit) */

#define __NR_read 3
__SYSCALL(__NR_read, sys_m_read)

/* fs/ioctl.c */
#define __NR_ioctl 54
__SYSCALL(__NR_ioctl, sys_m_ioctl)

#define __NR_writev 146
__SYSCALL(__NR_writev, sys_m_writev)

/* #include <uapi/asm-generic/unistd.h> */

#undef __NR_syscalls
#define __NR_syscalls 294

#undef __SYSCALL

#endif /* !defined(ARCH_WASM32_UNISTD_H) || defined(__SYSCALL) */
