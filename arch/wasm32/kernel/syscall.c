/*
 * Metal - A webassembly kernel
 * Copyright (C) 2018 Paulo André Azevedo Quirino
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

/* #include "metal/syscall.h" */
#include <asm/syscall_wrapper.h>
#include <linux/syscalls.h>
#include <asm/errno.h>
#include <asm/uaccess.h>
#include <asm/uio.h>

METAL_SYSCALL_DEFINE(3, read, unsigned int, d, char __user *, buf, size_t, count) {
	char buf2[count];
	size_t r = read(d, buf2, count);
	raw_copy_to_user(buf, buf2, r);
	return r;
}

/* SYSCALL_DEFINE1(close, unsigned int, fd) { */
/* 	return close(fd); */
/* } */

/* SYSCALL_DEFINE3(write, unsigned int, fd, const char __user *, buf, size_t, len) { */
/* 	const char buf2[len]; */
/* 	raw_copy_from_user((void*) buf2, buf, len); */
/* 	return write(fd, buf2, len); */
/* } */

METAL_SYSCALL_DEFINE(3, writev,
		unsigned long , fd,
		const struct iovec __user *, vec,
		unsigned long, vlen)
{
	int i;
	struct kvec kiov[vlen];
	char buff[255], *b;

	raw_copy_from_user(kiov, vec, sizeof(kiov));

	for (b = buff, i = 0; i < vlen; i++) {
		size_t l = kiov[i].iov_len;

		raw_copy_from_user(b, kiov[i].iov_base, l);
		kiov[i].iov_base = b;

		b += l;
	}

	return writev(fd, kiov, vlen);
}
