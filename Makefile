.SUFFIXES: .a
# .POSIX:

srcdir=${PWD}
linux-srcdir=${HOME}/linux
include ../wasm.mk
include scripts/Makefile.common

WASMBROWSER?=chrome

init-y			:= init/
drivers-y		:= drivers/
libs-y			:= lib/
core-y			:=
core-y			+= kernel/ mm/ fs/ security/
dummy-dirs := fs/exofs

include arch/${ARCH}/Makefile

vmlinux-dirs		:= ${init-y} ${drivers-y} ${libs-y} ${core-y}
vmlinux-dep		:= ${vmlinux-dirs:%/=%-dep}
vmlinux-clean		:= ${vmlinux-dirs:%/=%-clean}
vmlinux-cleandep	:= ${vmlinux-dirs:%/=%-cleandep}
vmlinux-dirs		:= ${vmlinux-dirs:%/=%}
init-y			:= ${init-y:%/=%/built-in.a}
core-y			:= ${core-y:%/=%/built-in.a}
drivers-y		:= ${drivers-y:%/=%/built-in.a}
libs-y1			:= ${libs-y:%/=%/lib.a}
libs-y2			:= ${libs-y:%/=%/built-in.a}

KBUILD_VMLINUX_INIT := ${head-y} ${init-y}
KBUILD_VMLINUX_MAIN := ${core-y} ${libs-y2} ${drivers-y} ${net-y} ${virt-y}
KBUILD_VMLINUX_LIBS := ${libs-y1}

# KBUILD_LDS          := arch/${ARCH}/kernel/vmlinux.lds

vmlinux-deps := $(KBUILD_LDS) $(KBUILD_VMLINUX_INIT) $(KBUILD_VMLINUX_MAIN) \
	$(KBUILD_VMLINUX_LIBS)

all: ${dummy-dirs} prepare ${vmlinux-dirs} metal.wasm

$(dummy-dirs):
	mkdir -p $@

LINK.o += --export __syscall0 --export __syscall1 --export __syscall2 --export __syscall3 \
	--export __syscall4 --export __syscall5 --export __syscall6

metal.wasm: built-in.a
	${LINK.o} -o $@ ${LDLIBS} -e start_kernel -whole-archive built-in.a

built-in.a: $(vmlinux-deps)
	@[[ "${Q2}" == "@" ]] || echo AR -o $@ ${vmlinux-dirs:%=%/}
	@{ echo create built-in.a ; ${vmlinux-deps:%=echo addlib % ; } \
		echo save ; echo end ; } | ${AR} -M

$(vmlinux-dirs): FORCE
	@[[ "$Q" == "@" ]] || echo cd-push $@
	@${MAKE} -C $@ ${MAKEFLAGS}
	@echo cd-pop

autoconf=include/generated/autoconf.h

prepare: ${autoconf}

config=scripts/.config

$(autoconf): ${config} extra-conf.h
	${Q2}{ sed '/^#/d' ${config} | while IFS="=" read name val; do \
		[[ "$$val" == "y" ]] && echo "#define $$name 1" || \
		[[ "$$val" == "n" ]] || echo "#define $$name $$val" ; \
		done ; cat extra-conf.h ; } > $@

.PHONY: all prepare

clean: ${vmlinux-clean}
	${Q2}rm metal.wasm *.a 2>/dev/null || true

$(vmlinux-clean):
	${Q2}${MAKE} -C ${@:%-clean=%} ${MAKEFLAGS} clean

.PHONY: clean ${vmlinux-clean}

dep: ${vmlinux-dep}

$(vmlinux-dep):
	@${MAKE} -C ${@:%-dep=%} ${MAKEFLAGS} dep

.PHONY: dep ${vmlinux-dep}

cleandep: ${vmlinux-cleandep}

$(vmlinux-cleandep):
	${Q2}${MAKE} -C ${@:%-cleandep=%} ${MAKEFLAGS} cleandep

.PHONY: cleandep ${vmlinux-cleandep}

install: all
	${Q2}cp metal.wasm ${DESTDIR}/bin

# naive approach for now.
tags:
	${Q2}${CTAGS} `find ${vmlinux-dirs} include arch/${ARCH}/include \
	    -name '*.c' -or -name '*.h'`

relink: broken
	cat broken | while read file ; do ln -fs ${linux-srcdir}/$$file ./$$file ; done

broken:
	find . -type l -exec file {} \; | grep broken | sed -e 's/^.\///' -e 's/:.*//' >$@

.PHONY: run install tags debug relink
