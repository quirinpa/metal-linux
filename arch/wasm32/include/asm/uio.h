#ifndef METAL_UIO_H
#define METAL_UIO_H

#include <linux/uio.h>

long write(int, const void*, unsigned long);
long writev(int, const struct kvec *, int);
long read(int, void *, unsigned long);
long close(int fd);

#endif
