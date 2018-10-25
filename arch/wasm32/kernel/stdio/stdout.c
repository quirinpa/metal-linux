#include "../stdio.h"

static unsigned char buf[BUFSIZ];
static struct file f = {
	.private_data = buf,
	.f_pos = 0,
};
struct file *const stdout = &f;
