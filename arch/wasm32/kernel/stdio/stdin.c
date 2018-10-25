#include "../stdio.h"

stdin_data_t stdin_data = { .rem = 0 };

static struct file f = {
	.private_data = &stdin_data,
	.f_pos = 0,
};

struct file * const stdin = &f;
