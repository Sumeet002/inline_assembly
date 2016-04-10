#include <stdio.h>
#include <stdint.h>

static inline void CopyInt32(uint32_t *addr,uint16_t inVal) __attribute__((always_inline,unused));

static inline void CopyInt32(uint32_t *addr , uint16_t inVal)
{

	asm("str %[value],[%[result]]\n\t"
		://outputs
		:[result]"r" (addr),[value]"r" (inVal) //inputs
		://clobbers
		);

}

uint32_t outValInt32;
void main()
{

	CopyInt32(&outValInt32,0x1234);
	printf("out value:%x",outValInt32);
}

