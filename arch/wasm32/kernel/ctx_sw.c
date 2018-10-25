/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Vineetg: Aug 2009
 *  -"C" version of lowest level context switch asm macro called by schedular
 *   gcc doesn't generate the dward CFI info for hand written asm, hence can't
 *   backtrace out of it (e.g. tasks sleeping in kernel).
 *   So we cheat a bit by writing almost similar code in inline-asm.
 *  -This is a hacky way of doing things, but there is no other simple way.
 *   I don't want/intend to extend unwinding code to understand raw asm
 */

#include <asm/asm-offsets.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/printk.h>

#define KSP_WORD_OFF 	((TASK_THREAD + THREAD_KSP) / 4)

struct task_struct *__sched
__switch_to(struct task_struct *prev_task, struct task_struct *next_task)
{
	pr_info("__switch_to\n");
	return NULL;
}
