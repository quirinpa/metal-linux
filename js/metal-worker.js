/*
 * Metal - A webassembly kernel
 * Copyright (C) 2018 Paulo André Azevedo Quirino
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "wasm-load.js"

#if defined(NODE) && defined(OLD)
#define NOT_CLIENT
#endif

function _jsnstr(buf, l) {
        let s = "";
        let i;

        for (i = 0; i < l; i++)
                s += String.fromCharCode(buf[i]);

        return s;
}

function jsnstr(mem, p, l) {
        return _jsnstr(new Uint8Array(mem, p, l), l);
}

let memory;

function _console_log(p, l) {
        console.log(jsnstr(memory.buffer, p, l));
}

function bcpy(bufd, bufo, d, o, len) {
        try {
                (new Uint8Array(bufd, d, len))
                        .set(new Uint8Array(bufo, o, len));

                return len;
        } catch (e) {
                console.error(e);
                return -1;
        }
}

function bmemcpy(memd, memo, d, o, len) {
        return bcpy(memd.buffer, memo.buffer, d, o, len);
}

let progs = { 0: 'kern' }

function umem(pid, d, o, len) {
        return bmemcpy(memory, progs[pid].memory, d, o, len);
}

function kmem(pid, d, o, len) {
        return bmemcpy(progs[pid].memory, memory, d, o, len);
}

#ifdef NOT_CLIENT
const fs = require('fs');
const sleep = require('sleep');

process.stdin.setRawMode(true);
process.stdin.pause();

function tty_read(d, len) {
        const buf = new Uint8Array(memory.buffer, d, len);
        let r = -1;

        process.stdin.resume();
        try {
                r = fs.readSync(process.stdin.fd, buf, 0, len);
        } catch (e) { // mostly EAGAIN
                // r = -1;
        }
        process.stdin.pause();

        return r;
}

function js_shutdown(e) {
        process.exit(e);
}

function js_sleep(ms) {
        sleep.msleep(ms);
}

function flush(p, l) {
        return _console_log(p, l - 1);
}

function vanilla_run(path) {
        return booted.then(() => run(path));
}

function js_emem() {}
function evt_count() {}
function js_run() {}

module.exports = {
        run: vanilla_run,
        shutdown: js_shutdown,
};
#else
let shbuf;

function js_emem(ofs, p, p_len) {
        return bcpy(memory.buffer, shbuf, p, ofs, p_len);
}

function init(_shbuf) {
        shbuf = _shbuf;
        evt_n = new Int32Array(shbuf, 0, 1);
        self.onmessage = null;
        return _shbuf;
}

const got_events = new Promise(resolve => {
        self.onmessage = event => resolve(init(event.data));
});

let evt_n;

function evt_count(ms) {
        if (Atomics.wait(evt_n, 0, 0, ms) == 'timed-out')
                return -1;

        return Atomics.exchange(evt_n, 0, 0);
}

function js_run(ptr, len) {
        const buf = new Uint8Array(memory.buffer, ptr, len);
        const path = _jsnstr(buf, len);

        run(path);
}

function flush(p, l) {
        postMessage(jsnstr(memory.buffer, p, l));
}

function js_shutdown(e) {}
function js_sleep() {}
function tty_read() {}
#endif

const kenv = {
        _console_log,
        flush,
        umem,
        kmem,

        js_shutdown,
        js_sleep,
        tty_read,

        js_emem,
        evt_count,
        js_run,

        memory: new WebAssembly.Memory({
                initial: 1,
                maximum: 32,
        }),
};

let syscall;

const booted = wasm_load_inst(
#ifdef NODE
        __dirname + '/' +
#endif
        'metal.wasm', kenv
).then(exports => {
        memory = exports.memory;
        syscall = {
                __syscall0: exports.__syscall0,
                __syscall1: exports.__syscall1,
                __syscall2: exports.__syscall2,
                __syscall3: exports.__syscall3,
                __syscall4: exports.__syscall4,
                __syscall5: exports.__syscall5,
                __syscall6: exports.__syscall6,
        };

        exports.start_kernel();
#ifndef NOT_CLIENT
        evt_loop = exports.evt_loop;
        got_events.then(evt_loop);
#endif
});

let next_pid = 1;

function run(path) {
        let prog = {};
        let dead;

        progs[next_pid] = prog;
        next_pid++;

        return wasm_load_inst(path, syscall).then(exports => {
                dead = exports.dead;
                prog.memory = exports.memory;
                exports._start();
                dead();
        }).catch(err => {
                // 'unreachable executed'
                if (err.message == 'unreachable' && dead)
                        dead();
                else
#ifdef NOT_CLIENT
                        console.error(err);
        });
#else
                        setTimeout(function () { throw err; }, 0);
        }).finally(evt_loop);
#endif
}
