#include "kbd.h"
/* #include "evt.h" */

typedef unsigned short (*k_hand)(unsigned short, int);

/* static unsigned short key_maps[MAX_NR_KEYMAPS] = {}; */
static unsigned short key_maps[][128] = {{
        '\0', '', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
        '', '	', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        '\0', 'a', 's', 'd', 'f', 'g', 'h', 'i', 'j', 'k', 'l', '\'', '`',
        0x101, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x101, '\0',
        '\0', ' ', '\0',
}, {
        '\0', '', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
        '', '	', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
        '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'I', 'J', 'K', 'L', '"', '~',
        0x101, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0x101, '\0',
        '\0', ' ', '\0',
}};

static unsigned shift_state = 0;

extern long write(int fd, const void * buf, unsigned long len);

static unsigned short
do_self(unsigned short ksym, int up_key)
{
        if (!up_key)
                write(0, &ksym, 1);
        return ksym;
}

static unsigned short
do_shift(unsigned short ksym, int up_key)
{
        shift_state ^= ksym & 0x000f;
        return ksym;
}

unsigned short do_ignore(unsigned short ksym, int up_key) { return -1; }

/* static k_hand key_handler[16] = { */
/*      do_self, do_fn, do_spec, do_pad, do_dead, */
/*      do_cons, do_cur, do_shift, do_meta, do_ascii, */
/*      do_lock, do_lowercase, do_ignore, do_ignore, */
/*      do_ignore, do_ignore */
/*  }; */

static k_hand key_handler[4] = {
	&do_self, &do_shift, &do_ignore
};

#define KTYP(keysym) ((keysym & 0xff00) >> 8)

void
input_kc(unsigned char kc, unsigned char up_flag)
{
	unsigned short keysym;
	unsigned char type;

	keysym = key_maps[shift_state][kc];
	type = KTYP(keysym);
	(*key_handler[type])(keysym & 0xff, up_flag);
}
