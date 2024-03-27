#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
/*	printf("\nPlease implement page_translate()\n");
	fflush(stdout);
	assert(0);*/
	uint32_t dir=(laddr>>22)&0x3ff;//目录号
	uint32_t page=(laddr>>12)&0x3ff;//页号
	uint32_t offset=laddr&0xfff;//页内偏移量
	
	uint32_t ptr=cpu.cr3.pdbr;
	
	PDE dir1;
	dir1.val=paddr_read((ptr<<12)+dir*4,4);
	assert(dir1.present);
	ptr=dir1.page_frame;
	
	PTE entry1;
	entry1.val=paddr_read((ptr<<12)+page*4,4);
	assert(entry1.present);
	ptr=entry1.page_frame;
	return (ptr<<12)+offset;
	

	
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
