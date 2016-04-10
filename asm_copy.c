#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>

#include <fcntl.h>
#include <sys/mman.h>

#include <sys/stat.h>

static inline void copy(uint32_t *addr,uint32_t *addr1) __attribute__((always_inline,unused));

static inline void copy(uint32_t *addr,uint32_t *addr1){

	asm("ldr r0,[%[address]]\n\t"
	    //"ldr r1,[%[address]]\n\t"
	    "str r0,[%[address1]]\n\t"
	    //"ldr r2,=0x020e0230\n\t"
	  	://outputs
		:[address]"r"(addr),[address1]"r"(addr1)//inputs
		:"r0","r1","memory"//clobbers
	);

	//asm volatile("ldr r0,[r1]");
}



void main(){

	static uint32_t addr1[100];	
	volatile unsigned int alloc_mem_size,page_mask,page_size;
	unsigned int mem_size;
	mem_size=0x1000;

	int mem_dev=open("/dev/mem",O_RDWR | O_SYNC);
	if(mem_dev == -1){
		printf("ERROR!! NO MEMORY DEVICE\n");
	}

	page_size = sysconf(_SC_PAGESIZE);
	alloc_mem_size = (((mem_size/page_size)+1)*page_size);
	page_mask=(page_size-1);

	volatile uint32_t* addr=mmap(NULL,alloc_mem_size,PROT_READ|PROT_WRITE,MAP_SHARED,mem_dev,(0x020e022c & ~page_mask));

	//addr[0] = 0x1234;
	printf("%x\n",&addr[0]);
	printf("%x\n",&addr1[0]);
 	copy(addr,addr1);
	printf("0x%x\n",addr1[0]);


}
