#ifndef _MIRCOMEMCPY_H
#define _MIRCOMEMCPY_H

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

//判断x是否是2的次方
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))
//取a和b中最小值
#define min(a, b) (((a) < (b)) ? (a) : (b))


void micro1bytecpy(uint8_t *pt,uint8_t *ps,uint8_t len){

	    uint8_t *ptar = pt;
	    uint8_t *psrc = ps;
            uint8_t i = 0;
	    while(i<len){
	      *(ptar + i) = *(psrc + i) ;	
              i++;
	      printf("[cpy1]write srcaddr=0x%lx, taraddr=0x%lx\n",(ptar+i),(psrc+i));
	    }

}
void micro8bytecpy(uint64_t *pt,uint64_t *ps,uint16_t len){

	    uint64_t *ptar = pt;
	    uint64_t *psrc = ps;
            uint8_t i = 0;
	    while(i<len){
	      *(ptar + i) = *(psrc + i) ;	
              i++;
	      printf("[cpy8]write srcaddr=0x%lx, taraddr=0x%lx\n",(ptar+i),(psrc+i));
	    }
}
void micro4bytecpy(uint32_t *pt,uint32_t *ps,uint8_t len){

	    uint32_t *ptar = pt;
	    uint32_t *psrc = ps;
            uint8_t i = 0;
	    while(i<len){
	      *(ptar + i) = *(psrc + i) ;	
              i++;
	      printf("[cpy4]write srcaddr=0x%lx, taraddr=0x%lx\n",(ptar+i),(psrc+i));
	    }
}

void micromemcpy(void *tar,void *src,uint16_t size){
	
	uint16_t len = 0;
	uint8_t mod = 0;
	uint8_t  div4 = 0;
	len = size ;
	uint16_t div8 = len/8;

	if(div8 == 0){
	//len < 8byte
	   div4 = len/4;
	   mod = len%4;
	   micro4bytecpy((uint32_t *)tar,(uint32_t *)src,div4);
	   micro1bytecpy((uint8_t *)(tar + div4*sizeof(uint32_t)),(uint8_t *)(src + div4*sizeof(uint32_t)),mod);
	}else{
	//len > 8byte
	   micro8bytecpy((uint64_t *)tar,(uint64_t *)src,div8);
           div4 = (len%8)/4;
	   micro4bytecpy((uint32_t *)(tar + div8*sizeof(uint64_t)),(uint32_t *)(src + div8*sizeof(uint64_t)),div4);
           mod = len%4;
	   micro1bytecpy((uint8_t *)(tar + div8*sizeof(uint64_t) + div4*sizeof(uint32_t)),(uint8_t *)(src + div8*sizeof(uint32_t) + div4*sizeof(uint32_t)),mod);
	}

}



#endif
