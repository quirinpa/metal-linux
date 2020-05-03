#include <linux/string.h>
#include <linux/init.h>

/* TODO dragons */
// TODO llvm. please export usual symbols and let me use a custom linker script
/* extern unsigned long data_end; */
unsigned long _text = 0;
unsigned long _stext = 0;
unsigned long _etext = 64;

#define SDATA 64
#define EDATA 8192
unsigned long _sdata = 64;
unsigned long _edata = EDATA;
unsigned long __init_begin = SDATA + 1;
unsigned long __init_end = SDATA + 16484;
unsigned long _end = 32768;
unsigned long _sinittext;
unsigned long _einittext;
unsigned long __bss_start = EDATA;
unsigned long __bss_stop = 30000;

unsigned long jiffies;
unsigned long __start_rodata = 4048;
unsigned long __end_rodata = 8096;
unsigned long __init_end;
void memzero(void *mem, __kernel_size_t len)
{
	memset(mem, 0, len);
}
void ret_from_fork(void) {}
unsigned long init_stack;
unsigned long __stop___param;
unsigned long __stop___ex_table;
unsigned long __start___param;
unsigned long __start___ex_table;
unsigned long __setup_start;
unsigned long __setup_end;
unsigned long __sched_text_start;
unsigned long __sched_text_end;

initcall_entry_t __initcall_start[0];
initcall_entry_t __initcall0_start[0];
initcall_entry_t __initcall1_start[0];
initcall_entry_t __initcall2_start[0];
initcall_entry_t __initcall3_start[0];
initcall_entry_t __initcall4_start[0];
initcall_entry_t __initcall5_start[0];
initcall_entry_t __initcall6_start[0];
initcall_entry_t __initcall7_start[0];
initcall_entry_t __initcall_end[0];

unsigned long __cpuidle_text_start;
unsigned long __cpuidle_text_end;

initcall_entry_t __con_initcall[] = {};
initcall_entry_t __con_initcall_start[];
initcall_entry_t __con_initcall_end[];
/* initcall_entry_t * __con_initcall_start = __con_initcall; */
/* initcall_entry_t * __con_initcall_end = __con_initcall; */

//void __compiletime_assert_778() { }
