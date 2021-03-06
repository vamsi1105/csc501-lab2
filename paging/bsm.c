/* bsm.c - manage the backing store mapping*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

/*-------------------------------------------------------------------------
 * init_bsm- initialize bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_bsm()
{
	STATWORD ps;
	disable(ps);
	int i = 0;
	for(i=0;i<NUM_BS;i++)
	{
		bsm_tab[i].bs_status = BSM_UNMAPPED;
		bsm_tab[i].bs_pid = -1;
		bsm_tab[i].bs_sem = 0;
		bsm_tab[i].bs_npages = 0;
		bsm_tab[i].bs_vpno = BASE_VPAGE_NUM;
		bsm_tab[i].bs_id=i;
	}
	restore(ps);

	return OK;
}

/*-------------------------------------------------------------------------
 * get_bsm - get a free entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL get_bsm(bsd_t* avail)
{
	STATWORD ps;
	disable(ps);
	int i = 0;
	int bs_id_map =-1;
	
	for(i=0;i<NUM_BS;i++)
	{
#if DEBUG_PAGING
	kprintf("\n\n\n\t\t****[%s:%d]bsm_tab[%d].bs_status = %d****\n\n",__FILE__,__LINE__,i,bsm_tab[i].bs_status);
#endif

		if(bsm_tab[i].bs_status == BSM_UNMAPPED)
		{
			*avail=i;
			restore(ps);
			return OK;
		}
		if(bsm_tab[i].bs_id==i){
			bs_id_map=1;
		}
	}

	restore(ps);
	return SYSERR;
}


/*-------------------------------------------------------------------------
 * free_bsm - free an entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL free_bsm(int i)
{
}

/*-------------------------------------------------------------------------
 * bsm_lookup - lookup bsm_tab and find the corresponding entry
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_lookup(int pid, long vaddr, int* store, int* pageth)
{
	STATWORD ps;
	disable(ps);
	int i=0;
	virt_addr_t *virt_addr;
	unsigned int pg_offset;
	virt_addr = &vaddr;

	pg_offset = vaddr/NBPG;

	for(i=0; i<NUM_BS; i++)
	{
		if(bsm_tab[i].bs_pid == pid)
		{
			if(pg_offset < bsm_tab[i].bs_vpno )
				return SYSERR;
			if(pg_offset > bsm_tab[i].bs_vpno + bsm_tab[i].bs_npages)
				return SYSERR;
			*store = i;
			*pageth = pg_offset-bsm_tab[i].bs_vpno;
		}
	}
	restore(ps);
	return OK;
}


/*-------------------------------------------------------------------------
 * bsm_map - add an mapping into bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_map(int pid, int vpno, int source, int npages)
{
}



/*-------------------------------------------------------------------------
 * bsm_unmap - delete an mapping from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_unmap(int pid, int vpno, int flag)
{
}

