#ifndef METAL_MAN
#define METAL_MAN

#include <unistd.h>
#include <stdio.h>

#define MEXPORT __attribute__((used)) __attribute__ ((visibility ("default")))
MEXPORT void dead() {
	fclose(stdout);
}

#endif
