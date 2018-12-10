#include <lib.h>
#include <stdio.h>
#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>

#include <fcntl.h>
#include <stdarg.h>
#include <string.h>

int lsr(char* path) {
	message m;
	
	_loadname(name, &m);
	_syscall(VFS_PROC_NR,VFS_LSR, &m);
}