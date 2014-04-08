/**
 * example to show how to use assembly embedded C
 * this embedded assembly syntax is used in GNU AS
 *
 * @see <a href=“http://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C”>Using Inline Assembly in C</a>
 * @see <a href=“http://sourceware.org/binutils/docs-2.17/as/index.html”>Using as</a>
 */

#include <stdio.h>

int main(int argc, char** argv) {
	printf("hello, assembly\n");
//	int x = 19;
//	int y = 7;
//	int add, sub, mul, quo, rem;
	// add(long), output use register a, input x use register a, y use register b
//	__asm__ ( "addl %%ebx, %%eax;" : "=a" (add) : "a" (x), "b" (y) );
//	printf("%d + %d = %d\n", x, y, add);
//	return 0;
}

