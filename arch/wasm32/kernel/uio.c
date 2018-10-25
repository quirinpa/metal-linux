/*
 * Metal - A webassembly kernel
 * Copyright (C) 2018 Paulo André Azevedo Quirino
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <asm/uio.h>
#include "evt.h"
#include "stdio.h"
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/fs.h>

long
read0_evt(char *buf, unsigned long len)
{
	struct file *f = stdin;
	stdin_data_t *data = f->private_data;
	long ret;

	while (data->rem == 0)
                do_events();

	ret = data->rem;
	memcpy(buf, data->buf + f->f_pos, ret);
	f->f_pos += ret;
	data->rem = 0;

	return ret;
}

long
read(int fd, void *buf, unsigned long len)
{
        switch (fd) {
        case 0: return read0_evt(buf, len);
        default: return -EBADF;
        }
}

void flush(char *, unsigned long);

long
write(int fd, const void * buf, unsigned long len)
{
        switch (fd) {
        case 0: {
		struct file *f = stdin;
		stdin_data_t *data = f->private_data;

                memcpy(data->buf + f->f_pos, buf, len);
		data->rem += len;

                break;
        }

        case 1: {
		struct file *f = stdout;
		char *data = f->private_data;

		memcpy(data + f->f_pos, buf, len);
		f->f_pos += len;

                if (((char*)buf)[len - 1] == '\n') {
			flush(data, f->f_pos);
			f->f_pos = 0;
		}

                break;
        }

        default: return -EBADF;
        };

        return len;
}

long
writev(int fd, const struct kvec* iov, int iovcnt)
{
        const struct kvec* e = &iov[iovcnt];
	long l;
        int i;

        for (l = 0; iov < e; iov++) {
                unsigned long ll = iov->iov_len;
                long r = write(fd, iov->iov_base, ll);

		if (r < 0)
			return r;

		else if (r < ll)
                        return l + r;

                else
                        l += ll;
        }

        return l;
}

/* FILE *ufile(int fd); */

long
close(int fd)
{
	/* FILE *f = ufile(fd); */
	/* return f->close(f); */
	return -1;
}
