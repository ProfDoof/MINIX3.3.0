#include "fs.h"
#include "file.h"
#include "scratchpad.h"
#include "lock.h"
#include "vnode.h"
#include "vmnt.h"
#include "path.h"
#include "threads.h"
#include "fproc.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <sys/dirent.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/u64.h>
#include <minix/vfsif.h>
#include <minix/safecopies.h>
#include <minix/driver.h>
#include <minix/drvlib.h>
#include <minix/vtreefs.h>
#include <minix/syslib.h>
#include <minix/log.h>
#include <minix/mmio.h>
#include <minix/gpio.h>
#include <minix/padconf.h>
#include <minix/type.h>
#include <minix/board.h>

int do_lsr(void) {
	message m;
	int counter = 0;
	int dabool = 0;
	struct filp *f;
	struct filp *g;
	struct filp filps[NR_FILPS];
	struct fproc *fpf;
	char fullpath[PATH_MAX];
	struct vnode *vp;
	struct vmnt *vmp;
	struct lookup resolve;

	if (copy_path(fullpath, sizeof(fullpath)) != OK)
	{
		printf("Unable to find the file given\n");
		return 1;
	}	

	lookup_init(&resolve, fullpath, PATH_NOFLAGS, &vmp, &vp);
	resolve.l_vmnt_lock = VMNT_READ;
	resolve.l_vnode_lock = VNODE_OPCL;
	if ((vp = eat_path(&resolve, fp)) == NULL) {
		return 1;
	}

	for( f = &filp[0]; f < &filp[NR_FILPS]; f++ )
	{
		if( f->filp_count != 0 && f->filp_vno == vp )
		{
			filps[counter] = *f;
			counter++;
		}
	}

	printf("Found All Filps\nBegin Printing PIDs...\n");

	for( fpf = &fproc[0]; fpf < &fproc[NR_PROCS]; fpf++ )
	{
		dabool = 0;
		for( f = (fpf->fp_filp[0]); dabool == 0 && f < (fpf->fp_filp[128]); f++ )
		{
			for( g = &filps[0]; dabool == 0 && g < &filps[counter]; g++ )
			{
				if( f->filp_count != 0 && g->filp_count != 0 && f->filp_vno == g->filp_vno )
				{
					printf("%u ", fpf->fp_pid);
				}
			}
		}
	}
	printf("\nPrinted all PIDs\nBegin Printing Blocks...\n");
	m.m_fs_vfs_lookup.device = vp->v_dev;
	m.m_fs_vfs_lookup.inode  = vp->v_inode_nr;
	m.m_type=0;
	req_do_lsr(vp->v_fs_e,&m);
	printf("\n");
	return 0;
	
}