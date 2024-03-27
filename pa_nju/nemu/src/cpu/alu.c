#include "cpu/cpu.h"

//✨


/*
#define sign(x) ((uint32_t)(x) >> 31)
//#define sign_ext(x) ((int32_t)((int8_t)(x)))

inline uint32_t sign_ext(uint32_t x, size_t data_size)
{
        assert(data_size == 16 || data_size == 8 || data_size == 32);
        switch (data_size)
        {
        case 8:
                return (int32_t)((int8_t)(x & 0xff));
        case 16:
                return (int32_t)((int16_t)(x & 0xffff));
        default:
                return (int32_t)x;
        }
}

inline uint64_t sign_ext_64(uint32_t x, size_t data_size)
{
        assert(data_size == 16 || data_size == 8 || data_size == 32);
        switch (data_size)
        {
        case 8:
                return (int64_t)((int8_t)(x & 0xff));
        case 16:
                return (int64_t)((int16_t)(x & 0xffff));
        default:
                return (int64_t)((int32_t)x);
        }
}

*/

// CF contains information relevant to unsigned integers

//DEST ← DEST + SRC;
void set_CF_add(uint32_t result, uint32_t src, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = result < src; 
}

////DEST ← DEST + SRC + CF;
void set_CF_adc(uint32_t result, uint32_t src, size_t data_size)
{
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(cpu.eflags.CF==0)
     	cpu.eflags.CF = result < src; 
    else
       cpu.eflags.CF = result <= src; //因为CF==1,所以result不可能等于src，溢出
    
    
}
//DEST ← DEST - SRC;
void set_CF_sub(uint32_t dest, uint32_t src, size_t data_size) {
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(dest<src)
    	cpu.eflags.CF=1;
    else
        cpu.eflags.CF=0;
	
}
void set_CF_sbb(uint32_t dest, uint32_t src, size_t data_size) {
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(cpu.eflags.CF==0)
    	cpu.eflags.CF=(dest<src);
    else
        cpu.eflags.CF=(dest<=src);
	
}


void set_ZF(uint32_t result, size_t data_size) {
	result = result & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.ZF = (result == 0);
}

// SF and OF contain information relevant to signed integers
void set_SF(uint32_t result, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.SF = sign(result);
}

void set_PF(uint32_t result) {
    //😺后8位是否有偶数个1
    
    //只截取后八位用于比较
    result=result&0x00FF;
    int count=0;
    while(result>0){
        if(result%2==1)
            count++;
        result/=2;
    }
    if(count%2==0)
        cpu.eflags.PF=1;//偶数个1
    else if(count%2==1)
        cpu.eflags.PF=0;
    
} // 


void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
	switch(data_size) {
		case 8: 
			result = sign_ext(result & 0xFF, 8); 
			src = sign_ext(src & 0xFF, 8); 
			dest = sign_ext(dest & 0xFF, 8); 
			break;
		case 16: 
			result = sign_ext(result & 0xFFFF, 16); 
			src = sign_ext(src & 0xFFFF, 16); 
			dest = sign_ext(dest & 0xFFFF, 16); 
			break;
		default: break;// do nothing
	}
	if(sign(src) == sign(dest)) {
		if(sign(src) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	} else {
		cpu.eflags.OF = 0;
	}
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
    //DEST ← DEST - SRC;
	switch(data_size) {
		case 8: 
			result = sign_ext(result & 0xFF, 8); 
			src = sign_ext(src & 0xFF, 8); 
			dest = sign_ext(dest & 0xFF, 8); 
			break;
		case 16: 
			result = sign_ext(result & 0xFFFF, 16); 
			src = sign_ext(src & 0xFFFF, 16); 
			dest = sign_ext(dest & 0xFFFF, 16); 
			break;
		default: break;// do nothing
	}
	if(sign(src) != sign(dest)) { //+ - -; - - + ；result必须与dest相同哦
		if(sign(dest) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	} else if(sign(src) == sign(dest)) {//同号相减不可能溢出
		cpu.eflags.OF = 0;
	}
}




//DEST ← DEST + SRC;
uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
    uint32_t res=0;
    res=dest+src;
    set_CF_add(res,src,data_size);
    set_PF(res);
    // set_AF();
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_add(res,src,dest,data_size);
    
    return res & (0xFFFFFFFF >> (32 - data_size)); // 高位清零并返回

#endif
    
    

}
//DEST ← DEST + SRC+CF;
uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{

#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
    uint32_t res=0;
    res=dest+src+cpu.eflags.CF;
    
    set_CF_adc(res,src,data_size);
    set_PF(res);
    
    // set_AF();
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_add(res,src,dest,data_size);//adcy与add的溢出判断是一样的
    return res&(0xFFFFFFFF>>(32-data_size));
#endif

  
    
}


//DEST ← DEST - SRC;
uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res=0;

    res=dest-src;
    
    set_CF_sub(dest,src,data_size);
    set_PF(res);
    
    // set_AF();
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_sub(res,src,dest,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
    


}

 //返回 dest - src - CF，截取低 data_size 位，高位置零，并设置各标志位
uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res=0;
    res=dest-src-cpu.eflags.CF;
    
   /* uint32_t tmp=src+cpu.eflags.CF;
    set_CF_sub(dest,tmp,data_size);*/
    //❤️这一步可能出错：假如src=2^31-1,CF==1，那么tmp=-2^31,本来dest<src的会被判断为dest>src
    //最好不要对参数进行直接操作
    
    set_CF_sbb(dest,src,data_size);
    set_PF(res);
    
    // set_AF();
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_sub(res,src,dest,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
#endif

    
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint64_t res=0;
    if(data_size==32){
        res=(uint64_t)src*(uint64_t)dest;//❤️采用强制类型转换
        if((res&0xFFFFFFFF00000000)==0)
           cpu.eflags.OF=cpu.eflags.CF=0;
        else
            cpu.eflags.OF=cpu.eflags.CF=1;
    }
    else if(data_size==16){
        res=src*dest;//
        if((res&0xFFFF0000)==0)
           cpu.eflags.OF=cpu.eflags.CF=0;
        else
            cpu.eflags.OF=cpu.eflags.CF=1;
        
    }
     else if(data_size==8){
         res=src*dest;//不可能超过FFFF 4*4=16
        if((res&0xFF00)==0)
           cpu.eflags.OF=cpu.eflags.CF=0;
        else
            cpu.eflags.OF=cpu.eflags.CF=1;
        
    }
        
    return res;
#endif
/*✨有size三种：32,16,8
     1.设置res
     2.同时设置CF、OF

*/    
}
// 返回两个操作数带符号乘法的乘积
int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
    int64_t res=0;
    if(data_size==32){
        res=(int64_t)dest*(int64_t)src;
        
    }
    else{
        res=dest*src;
    }
    
    if(res>>data_size){
        cpu.eflags.CF=cpu.eflags.OF=1;
        
    }
    else{
        
        cpu.eflags.CF=cpu.eflags.OF=0;
    }
        
   return res;
	
	
	
	
	
#endif
}

// need to implement alu_mod before testing
//// 返回无符号除法 dest / src 的商，遇到 src 为0直接报错（对应Linux是Floating Point Exception）退出程序
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
    uint32_t res=0;
    assert(src!=0);
    res=dest/src;//res被转化成了64位？
    
    res=res&0xFFFFFFFF;
    return  res;
	 
	 
#endif
}

// need to implement alu_imod before testing

// 返回带符号除法 dest / src 的商，遇到 src 为0直接报错退出程序
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
    int32_t res=0;
    assert(src!=0);
    res=dest/src;//res被转化成了64位？
    res=res&0xFFFFFFFF;
    return  res;
    //  
 
#endif
}


//返回无符号模运算 dest % src 的结果
// 实际上，整数除法和取余运算是由div或idiv指令同时完成的，我们这里为了方便，把模运算单独独立了出来
uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	assert(src!=0);
	uint32_t res=dest%src;
	
	return res;
	
	
	
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	assert(src!=0);
	int32_t res=dest%src;
	
	return res;


#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else


	uint32_t res=0;
    res=dest&src;
    //CF = 0, OF = 0; 
    cpu.eflags.CF=0;
    cpu.eflags.OF=0;
    //PF, SF, and ZF as described in Appendix C
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    // set_AF();
   
    
    return res&(0xFFFFFFFF>>(32-data_size));



#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else


	
	uint32_t res=0;
    res=dest^src;
    //CF = 0, OF = 0; 
    cpu.eflags.CF=0;
    cpu.eflags.OF=0;
    //PF, SF, and ZF as described in Appendix C
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    // set_AF();
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	
	
	
	uint32_t res=0;
    res=dest|src;
    //CF = 0, OF = 0; 
    cpu.eflags.CF=0;
    cpu.eflags.OF=0;
    //PF, SF, and ZF as described in Appendix C
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    // set_AF();
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}
/*
(* COUNT is the second parameter *)
(temp) ← COUNT;
WHILE (temp ≠ 0)
DO
 IF instruction is SAL or SHL
 THEN CF ← high-order bit of r/m;
 (* Determine overflow for the various instructions *)
IF COUNT = 1
THEN
 IF instruction is SAL or SHL
 THEN OF ← high-order bit of r/m ≠ (CF);
*/
//😒😒😒😒😒😒😒😒😒😒
uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	uint32_t res=0;
	if(src==0){
	    return dest&(0xFFFFFFFF>>(32-data_size));}
	    
    uint32_t tmp2=dest>>(data_size-1);
	uint32_t tmp=dest<<(src-1);//这里如果src<1就会报错，故要在开头进行特殊情况判断
	res=tmp<<1;
	//如何取最后一次左移出去的数？
	/*
	1.直接符号扩展tmp
	2.向右移（data_size-1）个单位
	//OF：
	RCL、RCR、ROL、ROR、SHR 指令：最后一次移位后的符号位与最后一次的前一次移位后的符号位不同，OF=1
    SHL 指令：最后一次移位后的符号位与未移位的符号位不同时，OF=1

	*/
	cpu.eflags.CF=(tmp>>(data_size-1))%2;
	set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
    if(src==1){
        cpu.eflags.OF=(cpu.eflags.CF!=tmp2);
        
    }
	 return res&(0xFFFFFFFF>>(32-data_size));
	
#endif
}


/*
IF instruction = SAR
 THEN r/m ← r/m /2 (*Signed divide, rounding toward negative infinity*)
 IF instruction is SHR
 THEN OF ← high-order bit of operand;
 注意手册中提到移位指令只在单次移位时才会对OF位进行设置，在NEMU中我们忽略这个细节，不对移位操作后的OF位进行测试。
 SAR and SHR shift the bits of the operand downward.
 The low-orderbit is shifted into the carry flag. 
 The effect is to divide the operand by
*/
uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	
	uint32_t res=0;
	if(src==0){
	    return dest&(0xFFFFFFFF>>(32-data_size));}
	
	/*
	
	uint32_t inputb[] = {1, 2, 3, 4};
	uint32_t inputa[] = {0xffffffff, 0x0, 0x10101010};
	高位有乱七八糟的东西！
	*/
	dest=dest&(0xFFFFFFFF>>(32-data_size));//先高位清零再右移，不然可能会出问题？？不加这行报错
    uint32_t tmp=dest>>(src-1);
    cpu.eflags.CF=tmp%2;
	res=tmp>>1;


	set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
  
	 return res&(0xFFFFFFFF>>(32-data_size));
	
	
	
	
	
#endif
}

// 返回将 dest 算术右移 src 位后的结果（高位补符）， data_size 用于指明操作数长度，标志位设置参照手册说明
uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else

//try First:
//错误原因：
//1.这个方法并不能补上符号位，还会让结果出错
//2.if else的嵌套循环关系也不对，不及时返回
//3.sign_dest不能在一开始就确定，万一有src>=data的情况出现
//4.忘记设置CF
   /* 
    dest=dest&(0xFFFFFFFF>>(32-data_size));
    uint32_t res=0;
    uint32_t sign_dest=(dest>>(data_size-1))%2;
	if(src==0){
	    res=dest;
	    
	}
	else if(src>=data_size)
	{
	    if(sign_dest==0){
            res=0x00000000;
        }
        else if(sign_dest==1){
         res=0xFFFFFFFF;
        }

	}
	
    uint32_t tmp=dest>>(src-1);
    cpu.eflags.CF=tmp%2;
    if(sign_dest==0){
        res=tmp>>1;
    }
    else if(sign_dest==1){
     res=tmp>>1;
     res=res&(0xFFFFFFFF<<(data_size-src));
    }
*/
    uint32_t res=0;
    dest=dest&(0xFFFFFFFF>>(32-data_size));
  
	if(src==0){
	    res=dest;
	    
	}
	
	else{
	     switch(data_size){
	         case 8:{
	             
	            int8_t tmp=(int8_t)dest>>(src-1);
	            cpu.eflags.CF=(tmp%2);
	            res=tmp>>1;
	            break;
	         }
	          case 16:{
	            int16_t tmp=(int16_t)dest>>(src-1);
	            cpu.eflags.CF=(tmp%2);
	            res=tmp>>1;
	            break;
	         }
	          case 32:{
	            int32_t tmp=(int32_t)dest>>(src-1);
	            cpu.eflags.CF=(tmp%2);
	            res=tmp>>1;
	            break;
	         }
	         
	         default:
	             break;
	         
	     }
	     
	    
	}
	

	set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
  
	 return res&(0xFFFFFFFF>>(32-data_size));
     
     
#endif
}

//😒😒😒😒😒😒😒😒😒😒
// 返回将 dest 算术左移 src 位后的结果， data_size 用于指明操作数长度（比特数），
// 可以是 8、16、32 中的一个用于判断标志位的取值，标志位设置参照手册说明
uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	uint32_t res=0;
	if(src==0){
	    return dest&(0xFFFFFFFF>>(32-data_size));}
	    
    uint32_t tmp2=dest>>(data_size-1);
	uint32_t tmp=dest<<(src-1);//这里如果src<1就会报错，故要在开头进行特殊情况判断
	res=tmp<<1;
	//如何取最后一次左移出去的数？
	/*
	1.直接符号扩展tmp
	2.向右移（data_size-1）个单位
	//OF：
	RCL、RCR、ROL、ROR、SHR 指令：最后一次移位后的符号位与最后一次的前一次移位后的符号位不同，OF=1
    SHL 指令：最后一次移位后的符号位与未移位的符号位不同时，OF=1

	*/
	cpu.eflags.CF=(tmp>>(data_size-1))%2;
	set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
    if(src==1){
        cpu.eflags.OF=(cpu.eflags.CF!=tmp2);
        
    }
	 return res&(0xFFFFFFFF>>(32-data_size));

	
	
	
	
#endif
}
