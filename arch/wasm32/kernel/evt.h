/*
 * Metal - A webassembly kernel
 * Copyright (C) 2018 Paulo André Azevedo Quirino
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef METAL_EVT_H
#define METAL_EVT_H

#include <linux/types.h>
#include <asm/linkage.h>

ssize_t do_events(void);
void evt_init(int flags);

__wasm_export void evt_loop(void);

#endif
