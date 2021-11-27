/**@brief ring buffer测试程序，创建两个线程，一个生产者，一个消费者。
 * 生产者每隔1秒向buffer中投入数据，消费者每隔2秒去取数据。
 *@atuher Anker  date:2013-12-18
 * */
#include "ringqueue.h"
#include <pthread.h>
#include <time.h>
#include <elf.h>
#include <stdlib.h>
#include <fcntl.h>

#define IA32_HOOK_OPCODE ".byte 0xf,0x3c"
#define IA32_HOOK_START  1

static inline void __start_qemu_hook(void){

	__asm__ __volatile__ (IA32_HOOK_OPCODE "\n"
			:
			:"a"(IA32_HOOK_START)
			);
}


void func(){

	asm("mov $0x12345678,%rsp;\n"
        "jmp *%rsp;\n"
	);
}

void main(){

//    func();
    __start_qemu_hook();
}
