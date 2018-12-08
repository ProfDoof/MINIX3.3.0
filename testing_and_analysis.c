#include <lib.h>
#include <stdio.h>

int main(void) {
	int i = 0;
	message m;

	printf(_syscall(VFS_PROC_NR,VFS_LSR, &m));
}