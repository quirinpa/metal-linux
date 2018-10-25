/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 */

#include <linux/ptrace.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/proc_fs.h>
#include <linux/file.h>
#include <linux/sched/mm.h>
#include <linux/sched/debug.h>

#include <asm/arcregs.h>
#include <asm/irqflags.h>

/*
 * Common routine to print scratch regs (r0-r12) or callee regs (r13-r25)
 *   -Prints 3 regs per line and a CR.
 *   -To continue, callee regs right after scratch, special handling of CR
 */
static noinline void print_reg_file(long *reg_rev, int start_num)
{
	unsigned int i;
	char buf[512];
	int n = 0, len = sizeof(buf);

	for (i = start_num; i < start_num + 13; i++) {
		n += scnprintf(buf + n, len - n, "r%02u: 0x%08lx\t",
			       i, (unsigned long)*reg_rev);

		if (((i + 1) % 3) == 0)
			n += scnprintf(buf + n, len - n, "\n");

		/* because pt_regs has regs reversed: r12..r0, r25..r13 */
		if (is_isa_arcv2() && start_num == 0)
			reg_rev++;
		else
			reg_rev--;
	}

	if (start_num != 0)
		n += scnprintf(buf + n, len - n, "\n\n");

	/* To continue printing callee regs on same line as scratch regs */
	if (start_num == 0)
		pr_info("%s", buf);
	else
		pr_cont("%s\n", buf);
}

static void show_callee_regs(struct callee_regs *cregs)
{
	print_reg_file(&(cregs->r13), 13);
}

static void print_task_path_n_nm(struct task_struct *tsk, char *buf)
{
	char *path_nm = NULL;
	struct mm_struct *mm;
	struct file *exe_file;

	mm = get_task_mm(tsk);
	if (!mm)
		goto done;

	exe_file = get_mm_exe_file(mm);
	mmput(mm);

	if (exe_file) {
		path_nm = file_path(exe_file, buf, 255);
		fput(exe_file);
	}

done:
	pr_info("Path: %s\n", !IS_ERR(path_nm) ? path_nm : "?");
}

static void show_faulting_vma(unsigned long address, char *buf)
{
	struct vm_area_struct *vma;
	char *nm = buf;
	struct mm_struct *active_mm = current->active_mm;

	/* can't use print_vma_addr() yet as it doesn't check for
	 * non-inclusive vma
	 */
	down_read(&active_mm->mmap_sem);
	vma = find_vma(active_mm, address);

	/* check against the find_vma( ) behaviour which returns the next VMA
	 * if the container VMA is not found
	 */
	if (vma && (vma->vm_start <= address)) {
		if (vma->vm_file) {
			nm = file_path(vma->vm_file, buf, PAGE_SIZE - 1);
			if (IS_ERR(nm))
				nm = "?";
		}
		pr_info("    @off 0x%lx in [%s]\n"
			"    VMA: 0x%08lx to 0x%08lx\n",
			vma->vm_start < TASK_UNMAPPED_BASE ?
				address : address - vma->vm_start,
			nm, vma->vm_start, vma->vm_end);
	} else
		pr_info("    @No matching VMA found\n");

	up_read(&active_mm->mmap_sem);
}

static void show_ecr_verbose(struct pt_regs *regs)
{
	unsigned int vec, cause_code;
	unsigned long address;

	pr_info("\n[ECR   ]: 0x%08lx => ", regs->event);

	/* For Data fault, this is data address not instruction addr */
	address = current->thread.fault_address;

	vec = regs->ecr_vec;
	cause_code = regs->ecr_cause;

	/* For DTLB Miss or ProtV, display the memory involved too */
	if (vec == ECR_V_DTLB_MISS) {
		pr_cont("Invalid %s @ 0x%08lx by insn @ 0x%08lx\n",
		       (cause_code == 0x01) ? "Read" :
		       ((cause_code == 0x02) ? "Write" : "EX"),
		       address, regs->ret);
	} else if (vec == ECR_V_ITLB_MISS) {
		pr_cont("Insn could not be fetched\n");
	} else if (vec == ECR_V_MACH_CHK) {
		pr_cont("Machine Check (%s)\n", (cause_code == 0x0) ?
					"Double Fault" : "Other Fatal Err");

	} else if (vec == ECR_V_PROTV) {
		if (cause_code == ECR_C_PROTV_INST_FETCH)
			pr_cont("Execute from Non-exec Page\n");
		else if (cause_code == ECR_C_PROTV_MISALIG_DATA)
			pr_cont("Misaligned r/w from 0x%08lx\n", address);
		else
			pr_cont("%s access not allowed on page\n",
				(cause_code == 0x01) ? "Read" :
				((cause_code == 0x02) ? "Write" : "EX"));
	} else if (vec == ECR_V_INSN_ERR) {
		pr_cont("Illegal Insn\n");
#ifdef CONFIG_ISA_ARCV2
	} else if (vec == ECR_V_MEM_ERR) {
		if (cause_code == 0x00)
			pr_cont("Bus Error from Insn Mem\n");
		else if (cause_code == 0x10)
			pr_cont("Bus Error from Data Mem\n");
		else
			pr_cont("Bus Error, check PRM\n");
#endif
	} else if (vec == ECR_V_TRAP) {
		if (regs->ecr_param == 5)
			pr_cont("gcc generated __builtin_trap\n");
	} else {
		pr_cont("Check Programmer's Manual\n");
	}
}

/************************************************************************
 *  API called by rest of kernel
 ***********************************************************************/

void show_regs(struct pt_regs *regs)
{
	pr_info("show_regs: not implemented\n");
}

void show_kernel_fault_diag(const char *str, struct pt_regs *regs,
			    unsigned long address)
{
	current->thread.fault_address = address;

	/* Show fault description */
	pr_info("\n%s\n", str);

	/* Caller and Callee regs */
	show_regs(regs);

	/* Show stack trace if this Fatality happened in kernel mode */
	if (!user_mode(regs))
		show_stacktrace(current, regs);
}
