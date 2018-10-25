#ifndef METAL_STDIO_H
#define METAL_STDIO_H

#define BUFSIZ 1024

#include <linux/fs.h>

typedef struct {
	loff_t rem;
	char buf[BUFSIZ];
} stdin_data_t;

extern struct file *const stdin;
extern struct file *const stdout;

#define stdin  (stdin)
#define stdout (stdout)

#endif
