#include <lib.h>
#include <stdio.h>
#include <sys/cdefs.h>
#include "namespace.h"

#include <fcntl.h>
#include <stdarg.h>
#include <string.h>

int lsr(char* path) {
	message m;
	
	_loadname(path, &m);
	_syscall(VFS_PROC_NR,VFS_LSR, &m);
}