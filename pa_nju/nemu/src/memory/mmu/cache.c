
#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include<time.h>
#include<nemu.h>
#include<stdlib.h>

//自定义宏======================
#define CacheLineSize 1024
#define AddrLineSize 64
//==============================



typedef struct CacheLine
{
    uint8_t validbit;//有效位
    uint32_t sign;//标志位
    uint8_t dataBlock[64];//cache block存储空间的大小为64B
    
}CacheLine;

//初始化空间 
struct CacheLine cache[CacheLineSize];
//标记位  cache组号(7位） 块内地址(6位)
void init_cache(){
    for(int i=0;i<CacheLineSize;++i){
        cache[i].validbit=0;
        cache[i].sign=0;
    }
    
}

//read时找不到就把cache存进去
void store(uint32_t paddr){
    int addrStart=paddr&0x3f;
    int line=-1;
    int groupId=(paddr >> 6) & 0x7f;
    int index=groupId*8;
    uint32_t sign=(paddr>>13)&0x7fff;//标记位
    //寻找空的cacheline
    for(int i=index;i<index+8;++i){
        if(!cache[i].validbit){
            line=i;
        }
    }
    if(line==-1){//没找到，采用随机数
        uint8_t kill=rand()%8;
        line=index+kill;
    }
    
    
    //存入cache
    paddr_t tmp=paddr-addrStart;
    memcpy(cache[line].dataBlock,hw_mem+tmp,AddrLineSize);
    cache[line].validbit=1;
    cache[line].sign=sign;
    
}

uint32_t cache_read(paddr_t paddr, size_t len){
    
   
    uint32_t addrStart=paddr&0x3f;//组内块号，相当于offset
    uint32_t groupId=(paddr>>6)&0x7f;//组数
    uint32_t sign=(paddr>>13)&0x7fff;//标记位
    
    //遍历数组
    for(int i=0;i<8;++i){
        //标记位相同 有效位为1
        if(cache[8*groupId+i].validbit&&cache[8*groupId+i].sign==sign){
            //判断是否跨块
            if(addrStart+len<64){//不跨块则直接读取
                uint32_t readVal=0;
                memcpy(&readVal,cache[8*groupId+i].dataBlock+addrStart,len);
                return readVal;
            }
            else {
                return hw_mem_read(paddr, len);

            }
        }
        
    }
    
    store(paddr);
    uint32_t ret=hw_mem_read(paddr,len);
    return ret;
    
    
}

void cache_write(paddr_t paddr, size_t len, uint32_t data){
      
    uint32_t addrStart=paddr&0x3f;//组内块号，相当于offset
    uint32_t groupId=(paddr>>6)&0x7f;//组数
    uint32_t sign=(paddr>>13)&0x7fff;//标记位
    
    //遍历数组
    for(int i=0;i<8;++i){
        //标记位相同 有效位为1
        if(cache[8*groupId+i].validbit&&cache[8*groupId+i].sign==sign){
            //判断是否跨块
            if(addrStart+len<64){
              hw_mem_write(paddr,len,data);
              memcpy(cache[8*groupId+i].dataBlock+addrStart,&data,len);
              return;
      
            }
            else {
              hw_mem_write(paddr,len,data);
              memcpy(cache[8*groupId+i].dataBlock+addrStart,&data,0x3f-addrStart);
              return;
            }
        }
        
    }
    
    hw_mem_write(paddr,len,data);
    store(paddr);
    
    
    
    
}
