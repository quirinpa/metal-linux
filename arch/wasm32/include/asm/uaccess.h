#ifndef ARCH_WASM_UACCESS_H
#define ARCH_WASM_UACCESS_H

unsigned long umem(int, char *, char *, unsigned long);
unsigned long kmem(int, char *, char *, unsigned long);

unsigned long __must_check __copy_user_ll
		(void *to, const void *from, unsigned long n);

static __always_inline unsigned long __must_check
raw_copy_to_user(void __user *to, const void *from, unsigned long n)
{
	return kmem(1, to, (char*) from, n);
}

static __always_inline unsigned long
raw_copy_from_user(void *to, const void __user *from, unsigned long n)
{
	return umem(1, (char*) to, (char*) from, n);
}

#include <asm-generic/uaccess.h>
#endif
