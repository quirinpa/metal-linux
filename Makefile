.SUFFIXES: .a .js
# .POSIX:

srcdir=${PWD}
include ../../wasm.mk
include scripts/Makefile.common

WASMBROWSER?=chrome

EXE = build/metal.wasm
init-y			:= init/
drivers-y		:= drivers/
libs-y			:= lib/
core-y			:=
core-y			+= kernel/ mm/ fs/ security/

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

WORKER=js/metal-worker.js
INTERF=js/metal.js
MW=build/m.w.js
MN=build/m.n.js
WW=build/w.w.js
WN=build/w.n.js
WON=build/w.o.n.js

all: prepare ${EXE} ${WON} ${WN} ${WW} ${MN} ${MW}

${EXE}: built-in.a
	${LINK.o} -o ${EXE} ${LDLIBS} -e start_kernel -whole-archive built-in.a

built-in.a: ${vmlinux-deps}
	${Q2}{ echo create built-in.a ; ${vmlinux-deps:%=echo addlib % ; } \
		echo save ; echo end ; } | ar -M

$(vmlinux-deps): FORCE
	${Q}${MAKE} -C ${@:%/built-in.a=%:%/lib.a=%} ${MAKEFLAGS}
	${Q2}echo cd `echo ${@:%/built-in.a=%:%/lib.a=%} | sed 's/[^/]*/../g'`

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
	${Q2}rm ${EXE} *.a build/*.js 2>/dev/null || true

$(vmlinux-clean):
	${Q2}${MAKE} -C ${@:%-clean=%} ${MAKEFLAGS} clean

.PHONY: clean ${vmlinux-clean}

dep: ${vmlinux-dep}

$(vmlinux-dep):
	${Q2}${MAKE} -C ${@:%-dep=%} ${MAKEFLAGS} dep

.PHONY: dep ${vmlinux-dep}

cleandep: ${vmlinux-cleandep}

$(vmlinux-cleandep):
	${Q2}${MAKE} -C ${@:%-cleandep=%} ${MAKEFLAGS} cleandep

.PHONY: cleandep ${vmlinux-cleandep}

$(MW): build ${INTERF}
	${Q2}${CPP} -P -o $@ -C ${INTERF}

$(MN): build ${INTERF}
	${Q2}${CPP} -P -o $@ -C -DNODE ${INTERF}

$(WW): build ${WORKER}
	${Q2}${CPP} -P -o $@ -C ${WORKER}

$(WN): build ${WORKER}
	${Q2}${CPP} -P -o $@ -C -DNODE ${WORKER}

$(WON): build ${WORKER}
	${Q2}${CPP} -P -o $@ -C -DNODE -DOLD ${WORKER}

build:
	${Q2}mkdir build

run:	all #${WN} ${MN} ${WON}
	${Q2}${MAKE} -C ../example run

install: all
	${Q2}cp ${EXE} ${sysroot}/bin

# naive approach for now.
tags:
	${Q2}${CTAGS} `find ${vmlinux-dirs} include arch/${ARCH}/include \
	    -name '*.c' -or -name '*.h'`

.PHONY: run install tags debug
