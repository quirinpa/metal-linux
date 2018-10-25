#ifndef ARCH_WASM_SETUP_H
#define ARCH_WASM_SETUP_H

#include <uapi/asm-generic/setup.h>

#define IS_AVAIL1(v, s)		((v) ? s : "")
#define IS_DISABLED_RUN(v)	((v) ? "" : "(disabled) ")
#define IS_USED_RUN(v)		((v) ? "" : "(not used) ")
#define IS_USED_CFG(cfg)	IS_USED_RUN(IS_ENABLED(cfg))
#define IS_AVAIL2(v, s, cfg)	IS_AVAIL1(v, s), IS_AVAIL1(v, IS_USED_CFG(cfg))
#define IS_AVAIL3(v, v2, s)	IS_AVAIL1(v, s), IS_AVAIL1(v, IS_DISABLED_RUN(v2))

void __init setup_arch_memory(void);

#endif /* ARCH_WASM_SETUP_H */
