/*
 * Metal - A webassembly kernel
 * Copyright (C) 2018 Paulo André Azevedo Quirino
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "evt.h"
#include <asm/uio.h>
#include "kbd.h"

ssize_t js_emem(size_t, u8 *, size_t);

static ssize_t
emem(u8 * data, size_t len)
{
	/* static unsigned const end = 2048; */
	static unsigned const start = 4;
	static unsigned ofs = start;
        ssize_t r = js_emem(ofs, data, len);
        ofs += r;
        return r;
}

static ssize_t
emem_str(char *data) {
	u8 len;
	emem(&len, sizeof(len));
	emem((u8*) data, len);
	return len + sizeof(len);
}

typedef void (*evt_cb_t)(void);
#ifdef CONFIG_WASM32_WEB_KBD
static void do_write() {
        u8 key_up, code;
        emem(&key_up, sizeof(key_up));
        emem(&code, sizeof(code));
        input_kc(code, key_up);
}
#else
ssize_t tty_read(char *, size_t); // js

static void do_write() {
	char buf[256];
	ssize_t len = tty_read(buf, sizeof(buf));
        if (len > 0)
                write(0, buf, len);
}
#endif

void js_run(char *path, size_t len);
static void do_run() {
        char path[256];
	u8 len = emem_str(path);
        js_run(path, len - sizeof(len));
}

void js_shutdown(int);
void do_shutdown() {
	u8 ret;
	emem(&ret, sizeof(ret));
	js_shutdown(ret);
}

static evt_cb_t cb[7] = {
        0,
        &do_write,
        0,
        0,
        &do_run,
        0,
        &do_shutdown,
};


static unsigned wait = 100;

#ifdef CONFIG_WASM32_WEB
ssize_t evt_count(unsigned);

ssize_t do_events() {
        ssize_t n = evt_count(wait), i, j;

        for (i = 0; i < n; i++) {
                u8 flags;
                emem(&flags, sizeof(flags));
                cb[flags]();
        }

        return n;
}
#else

void js_sleep(unsigned);

ssize_t do_events() {
        char buf[256];
        char buf2[256];
        ssize_t r;
        int i, j;

        while ((r = tty_read(buf, sizeof(buf))) < 0)
                js_sleep(wait);

	for (i = j = 0; i < r; i++) switch (buf[i]) {
	case 3:		js_shutdown(130);
	default:	buf2[j] = buf[i]; j++;
	}

        write(0, buf2, j);
        return 1;
}
#endif

__wasm_export void
evt_loop()
{
	// TODO loop until async call
        for (;!do_events(););
}
