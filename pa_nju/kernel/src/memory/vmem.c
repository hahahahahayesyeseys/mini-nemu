#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE) // number of page tables to cover the vmem
PTE tb[1024]  align_to_page;
PDE *get_updir();

void create_video_mapping()
{

 
    int j=0;
  PDE* p1=(PDE*)va_to_pa(get_updir());
 for(int i=0xa0;j<=0xf;++j,++i)
    {
       tb[0xa0+j].val=make_pte(i<<12);
    }
 p1[0].val=make_pde(va_to_pa(tb));
    

}

void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}
