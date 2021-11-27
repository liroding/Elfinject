#include "mircomemcpy.h"
#include "zx_qemu_hook_addr.h"

    uint16_t aa = 0x11;
void main(){

    uint8_t *mp = NULL;
    uint16_t i = 0;
    uint16_t bb = 0x11;
    mp = (uint8_t *)malloc(0x100);
    printf("mp addr=0x%lx\n",mp);
 //   __start_qemu_hook();
 //   __insert_hook_addr_list(mp,0x100);

 //   memset(mp,0x7f,0x100);

    for(i=0;i<0x100;i++)
    {
        sleep(10);
        *(mp + i) = 0x5f;
        aa = 0x6f;
        bb = 0x7f;
        //printf("test[%d] addr=0x%lx\n",i,mp+i);
    }

//    __stop_qemu_hook();
//    __delete_hook_addr_list(mp,0x100);

}
