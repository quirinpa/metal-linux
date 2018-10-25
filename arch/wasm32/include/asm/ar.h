/*
 * Metal - A webassembly kernel
 * Copyright (C) 2018 Paulo André Azevedo Quirino
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef AR_H
#define AR_H

// used in syscall definition, etc

#define AR_DECL(ar, ...) AR_DECL##ar(__VA_ARGS__)
#define AR_DECL0()
#define AR_DECL1(t, n) , t n
#define AR_DECL2(t, n, ...) , t n AR_DECL1(__VA_ARGS__)
#define AR_DECL3(t, n, ...) , t n AR_DECL2(__VA_ARGS__)
#define AR_DECL4(t, n, ...) , t n AR_DECL3(__VA_ARGS__)
#define AR_DECL5(t, n, ...) , t n AR_DECL4(__VA_ARGS__)
#define AR_DECL6(t, n, ...) , t n AR_DECL5(__VA_ARGS__)

#define AR_DECL_(ar, ...) AR_DECL_##ar(__VA_ARGS__)
#define AR_DECL_0()
#define AR_DECL_1(t, n) t n
#define AR_DECL_2(t, n, ...) t n, AR_DECL_1(__VA_ARGS__)
#define AR_DECL_3(t, n, ...) t n, AR_DECL_2(__VA_ARGS__)
#define AR_DECL_4(t, n, ...) t n, AR_DECL_3(__VA_ARGS__)
#define AR_DECL_5(t, n, ...) t n, AR_DECL_4(__VA_ARGS__)
#define AR_DECL_6(t, n, ...) t n, AR_DECL_5(__VA_ARGS__)

#define AR_PASS(ar, ...) AR_PASS##ar(__VA_ARGS__)
#define AR_PASS0()
#define AR_PASS1(t, n) , n
#define AR_PASS2(t, n, ...) , n AR_PASS1(__VA_ARGS__)
#define AR_PASS3(t, n, ...) , n AR_PASS2(__VA_ARGS__)
#define AR_PASS4(t, n, ...) , n AR_PASS3(__VA_ARGS__)
#define AR_PASS5(t, n, ...) , n AR_PASS4(__VA_ARGS__)
#define AR_PASS6(t, n, ...) , n AR_PASS5(__VA_ARGS__)

#define AR_PASS_(ar, ...) AR_PASS_##ar(__VA_ARGS__)
#define AR_PASS_0()
#define AR_PASS_1(t, n) n
#define AR_PASS_2(t, n, ...) n, AR_PASS_1(__VA_ARGS__)
#define AR_PASS_3(t, n, ...) n, AR_PASS_2(__VA_ARGS__)
#define AR_PASS_4(t, n, ...) n, AR_PASS_3(__VA_ARGS__)
#define AR_PASS_5(t, n, ...) n, AR_PASS_4(__VA_ARGS__)
#define AR_PASS_6(t, n, ...) n, AR_PASS_5(__VA_ARGS__)

#define AR_LTR0()
#define AR_LTR1(t, n) unsigned long, n
#define AR_LTR2(t, n, ...) unsigned long, n, AR_LTR1(__VA_ARGS__)
#define AR_LTR3(t, n, ...) unsigned long, n, AR_LTR2(__VA_ARGS__)
#define AR_LTR4(t, n, ...) unsigned long, n, AR_LTR3(__VA_ARGS__)
#define AR_LTR5(t, n, ...) unsigned long, n, AR_LTR4(__VA_ARGS__)
#define AR_LTR6(t, n, ...) unsigned long, n, AR_LTR5(__VA_ARGS__)

#define AR_MCAST0()
#define AR_MCAST1(t, n) , ((t) n)
#define AR_MCAST2(t, n, ...) , ((t) n) AR_MCAST1(__VA_ARGS__)
#define AR_MCAST3(t, n, ...) , ((t) n) AR_MCAST2(__VA_ARGS__)
#define AR_MCAST4(t, n, ...) , ((t) n) AR_MCAST3(__VA_ARGS__)
#define AR_MCAST5(t, n, ...) , ((t) n) AR_MCAST4(__VA_ARGS__)
#define AR_MCAST6(t, n, ...) , ((t) n) AR_MCAST5

#define AR_MCAST_(ar, ...) AR_MCAST_##ar(__VA_ARGS__)
#define AR_MCAST_0()
#define AR_MCAST_1(t, n) ((t) n)
#define AR_MCAST_2(t, n, ...) ((t) n), AR_MCAST_1(__VA_ARGS__)
#define AR_MCAST_3(t, n, ...) ((t) n), AR_MCAST_2(__VA_ARGS__)
#define AR_MCAST_4(t, n, ...) ((t) n), AR_MCAST_3(__VA_ARGS__)
#define AR_MCAST_5(t, n, ...) ((t) n), AR_MCAST_4(__VA_ARGS__)
#define AR_MCAST_6(t, n, ...) ((t) n), AR_MCAST_5(__VA_ARGS__)

// used in syscall declaration

#define AR_LGEN0
#define AR_LGEN1 unsigned long, a
#define AR_LGEN2 AR_LGEN1, unsigned long, b
#define AR_LGEN3 AR_LGEN2, unsigned long, c
#define AR_LGEN4 AR_LGEN3, unsigned long, d
#define AR_LGEN5 AR_LGEN4, unsigned long, e
#define AR_LGEN6 AR_LGEN5, unsigned long, f

#define AR_IDEN0()
#define AR_IDEN1(t, ...) t
#define AR_IDEN2(t, ...) t, AR_IDEN1(__VA_ARGS__)
#define AR_IDEN3(t, ...) t, AR_IDEN2(__VA_ARGS__)
#define AR_IDEN4(t, ...) t, AR_IDEN3(__VA_ARGS__)
#define AR_IDEN5(t, ...) t, AR_IDEN4(__VA_ARGS__)
#define AR_IDEN6(t, ...) t, AR_IDEN5(__VA_ARGS__)

#endif
