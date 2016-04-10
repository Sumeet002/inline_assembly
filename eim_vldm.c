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

	asm("pld [%[address]]\n\t"
	    "vldm %[address],{d0-d7}\n\t"
	    "vstm %[address1]!,{d0-d7}\n\t"
	    ://outputs
            :[address]"r"(addr),[address1]"r"(addr1)//inputs
	    :"memory"//clobbers
	);


}


void main(){



	static uint32_t addr1[100];	
	volatile unsigned int alloc_mem_size,page_mask,page_size;
	volatile uint32_t *addr;
	volatile uint32_t *mem_pointer;
	uint32_t mem_address; 
	unsigned int mem_size;
	mem_size=0x1000;
	mem_address=0x08000000;

	int mem_dev=open("/dev/mem",O_RDWR | O_SYNC);
	if(mem_dev == -1){
		printf("ERROR!! NO MEMORY DEVICE\n");
	}

	page_size = sysconf(_SC_PAGESIZE);
	alloc_mem_size = (((mem_size/page_size)+1)*page_size);
	page_mask=(page_size-1);

	mem_pointer=(uint32_t *)mmap(NULL,alloc_mem_size,PROT_READ|PROT_WRITE,MAP_SHARED,mem_dev,(mem_address & ~page_mask));

	if(mem_pointer == MAP_FAILED){
		printf("Error!! No pointer obtained");
	}
	addr= (mem_pointer +(mem_address & page_mask));
	
	printf("%x\n",&addr[0]);
	printf("%x\n",&addr1[0]);
	int i;

	copy(addr,addr1);
	
	i=0;
	for(i=0;i<16;i++){
	printf("0x%x\n",addr1[i]);
	}

}
